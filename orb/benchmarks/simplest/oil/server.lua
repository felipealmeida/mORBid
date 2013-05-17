
--io.stdout:setvbuf("no")

oil = require "oil"                                    -- Load OiL package
print "1"
parsed = require "parsed"
print "1"

oil.main(function()
	local simplest = {  }      -- Get object implementation

        assert ( _G["MORBID_IOR_PATH"] )

        local times = 0;

	function simplest:foo()
           times = times + 1
           if times == 1000000 then
              orb:stop()
           end
           return true
	end

	local orb = oil.init()
	
        local array = require "table"
        local unpack = array.unpack or _G.unpack
        orb.TypeRepository.registry:register(unpack(parsed))

	simplest = orb:newservant(simplest, nil, "simplest")                  -- Create CORBA object

	local ref = tostring(simplest)                -- Get object's reference
        print ("reference is ", ref)
	if not oil.writeto(_G["MORBID_IOR_PATH"], ref.."\n") then
		print(ref)
	end

	orb:run()                                      -- Start ORB main loop
end)
