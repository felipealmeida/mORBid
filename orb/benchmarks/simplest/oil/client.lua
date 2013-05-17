
--io.stdout:setvbuf("no")

oil = require "oil"                                             -- Load OiL package

parsed = require "parsed"

oil.main(function()
       local orb = oil.init()

        local array = require "table"

        local unpack = array.unpack or _G.unpack
        orb.TypeRepository.registry:register(unpack(parsed))

        assert ( _G["MORBID_IOR_PATH"] )
	
	simplest = orb:newproxy(assert(oil.readfrom(_G["MORBID_IOR_PATH"]))) -- Get proxy to object

        local before = os.clock()	
	for i = 0, 1000000 do
           simplest:foo() 
        end

	print("Finished in ", (os.clock() - before))
end)
