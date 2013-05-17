
--io.stdout:setvbuf("no")

oil = require "oil"                                             -- Load OiL package

parsed = require "parsed"

oil.main(function()
       local orb = oil.init()

        local array = require "table"

        assert ( _G["MORBID_IOR_PATH"] )
	
	simplest = orb:newproxy(assert(oil.readfrom(_G["MORBID_IOR_PATH"]))) -- Get proxy to object

        print "Starting"
        io.stdout:flush()

        local before = os.clock()	
	for i = 1, 10000 do
           simplest:foo() 
        end

	print("Finished in ", (os.clock() - before))
        io.stdout:flush()
end)
