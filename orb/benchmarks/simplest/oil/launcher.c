/*
** $Id: lua.c,v 1.160 2006/06/02 15:34:00 roberto Exp $
** Lua stand-alone interpreter
** See Copyright Notice in lua.h
*/


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void luapreload_extralibraries(lua_State*);

static lua_State *globalL = NULL;

static const char *progpath = NULL;

static const char *callerchunk =
" _G.lua51_pcall = _G.pcall"
" _G.lua51_xpcall = _G.xpcall"
" require 'coroutine.pcall'"

" local _loadfile = _G.loadfile"
" _G.lua51_loadfile = _loadfile"
" function _G.loadfile(path, mode, env)"
" 	local result, errmsg = _loadfile(path)"
" 	if result ~= nil and env ~= nil then"
" 		setfenv(result, env)"
" 	end"
" 	return result, errmsg"
" end"

" local coroutine = require 'coroutine'"
" local newthread = coroutine.create"
" local cothread = require 'cothread'"
" local step = cothread.step"
" local run = cothread.run"
" return function(f, ...)"
" 	return run(step(newthread(f), ...))"
" end";


static void lstop (lua_State *L, lua_Debug *ar) {
	(void)ar;  /* unused arg. */
	lua_sethook(L, NULL, 0, 0);
	luaL_error(L, "interrupted!");
}


static void laction (int i) {
	signal(i, SIG_DFL); /* if another SIGINT happens before lstop,
	                       terminate process (default action) */
	lua_sethook(globalL, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}


static void l_message (const char *pname, const char *msg) {
	if (pname) fprintf(stderr, "%s: ", pname);
	fprintf(stderr, "%s\n", msg);
	fflush(stderr);
}


static int report (lua_State *L, int status) {
	if (status && !lua_isnil(L, -1)) {
		const char *msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error object is not a string)";
		l_message(progpath, msg);
		lua_pop(L, 1);
	}
	return status;
}


static int traceback (lua_State *L) {
	lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
	if (lua_isfunction(L, -1)) {
		lua_insert(L, 1);  /* place below the message */
		lua_call(L, 1, 1);  /* call tostring */
	} else {
		lua_pop(L, 1);
	}
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1);  /* pass error message */
	lua_pushinteger(L, 2);  /* skip this function and traceback */
	lua_call(L, 2, 1);  /* call debug.traceback */
	return 1;
}


static int docall (lua_State *L, int narg) {
	int status;
	int base = lua_gettop(L) - narg;  /* function index */
	lua_pushcfunction(L, traceback);  /* push traceback function */
	lua_insert(L, base);  /* put it under chunk and args */
	signal(SIGINT, laction);
	status = lua_pcall(L, narg, 0, base);
	signal(SIGINT, SIG_DFL);
	lua_remove(L, base);  /* remove traceback function */
	/* force a complete garbage collection in case of errors */
	if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
	return status;
}


static int getargs (lua_State *L, char **argv) {
	int narg;
	int i;
	int argc = 0;
	while (argv[argc]) argc++;  /* count total number of arguments */
	narg = argc-1;  /* number of arguments to the script */
	luaL_checkstack(L, narg, "too many arguments to script");
	for (i=1; i < argc; i++) lua_pushstring(L, argv[i]);
	return narg;
}


struct Smain {
        int argc;
	char **argv;
	int status;
};


static int pmain (lua_State *L) {
	struct Smain *s = (struct Smain *)lua_touserdata(L, 1);
	char **argv = s->argv;
        int argc = s->argc;
	int status = 0;
	globalL = L;
	lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
	luaL_openlibs(L);  /* open libraries */
	lua_gc(L, LUA_GCRESTART, 0);
	
	if (argv[0] && argv[0][0]) {
		progpath = argv[0];
		lua_pushstring(L, progpath); lua_setglobal(L, "OPENBUS_PROGPATH");
	}
        if(argc > 1)
        {
          lua_pushstring(L, argv[1]); lua_setglobal(L, "MORBID_IOR_PATH");
        }
	
	/* preload libraries and global variables */
	luapreload_extralibraries(L);
	
	/* ??? */
	if (status == 0) {
		/* execute main module */
		status = luaL_dostring(L, callerchunk);
		if (status == 0) {
			lua_getglobal(L, "require");
#ifdef MORBID_OIL_SERVER
			lua_pushstring(L, "oil.server");
#else
			lua_pushstring(L, "oil.client");
#endif
			status = lua_pcall(L, 1, 1, 0);
			if (status == 0) {
				int narg = getargs(L, argv);  /* collect arguments */
				status = docall(L, narg+1);
			}
		}
	}
	s->status = report(L, status);
	return 0;
}


int main (int argc, char **argv) {
	int status;
	struct Smain s;
	lua_State *L = lua_open();  /* create state */
	if (L == NULL) {
		l_message(argv[0], "cannot create Lua state: not enough memory");
		return EXIT_FAILURE;
	}
        s.argc = argc;
	s.argv = argv;
	status = lua_cpcall(L, &pmain, &s);
	report(L, status);
	lua_close(L);
	return (status || s.status) ? EXIT_FAILURE : EXIT_SUCCESS;
}
