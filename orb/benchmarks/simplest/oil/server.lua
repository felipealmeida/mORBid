
--io.stdout:setvbuf("no")

oil = require "oil"                                    -- Load OiL package
--parsed = require "parsed"

start = os.clock()

ior_path = assert ( _G["MORBID_IOR_PATH"] or ... 
                  )


oil.main(function()

        --oil.verbose:level(2)
        --oil.verbose.viewer.output = assert(io.open("server.log", "w"))
        --oil.verbose.timed = true
        --local TimeFormat = "%d/%m/%Y %H:%M:%S"
        --oil.verbose:settimeformat(TimeFormat) -- inclui data e hora no log do OiL

        local times = 0;

	local orb = oil.init()

	local simplest = {  }      -- Get object implementation
	function simplest:foo()
           --print( "foo " , times)
           io.stdout:flush()
           times = times + 1
           if times == 1000000 then
             orb:shutdown()
           end
           return true
	end
	
        --local array = require "table"
        --local unpack = array.unpack or _G.unpack
        --orb.TypeRepository.registry:register(unpack(parsed))
        orb:loadidlfile('/home/felipe/dev/tecgraf/morbid/orb/benchmarks/simplest/simplest.idl')

	simplest = orb:newservant(simplest, nil, "simplest")                  -- Create CORBA object

	local ref = tostring(simplest)                -- Get object's reference
        print ("reference is ", ref)

        print ("Initialization time is ", (os.clock()-start))

	if not oil.writeto(ior_path, ref.."\n") then
		print(ref)
	end

        io.stdout:flush()

	orb:run()                                      -- Start ORB main loop

end)
