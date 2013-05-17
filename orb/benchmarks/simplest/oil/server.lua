
--io.stdout:setvbuf("no")

oil = require "oil"                                    -- Load OiL package
parsed = require "parsed"

start = os.clock()

oil.main(function()

        assert ( _G["MORBID_IOR_PATH"] )

        local times = 0;

	local orb = oil.init()

	local simplest = {  }      -- Get object implementation
	function simplest:foo()
           print( "foo " , times)
           io.stdout:flush()
           times = times + 1
           if times == 10000 then
             orb:shutdown()
           end
           return true
	end
	
        local array = require "table"
        local unpack = array.unpack or _G.unpack
        orb.TypeRepository.registry:register(unpack(parsed))

	simplest = orb:newservant(simplest, nil, "simplest")                  -- Create CORBA object

	local ref = tostring(simplest)                -- Get object's reference
        print ("reference is ", ref)

        print ("Initialization time is ", (os.clock()-start))

	if not oil.writeto(_G["MORBID_IOR_PATH"], ref.."\n") then
		print(ref)
	end

        io.stdout:flush()

	orb:run()                                      -- Start ORB main loop

end)
