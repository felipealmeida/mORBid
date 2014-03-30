
--io.stdout:setvbuf("no")

oil = require "oil"                                             -- Load OiL package
--parsed = require "parsed"

--parsed = require "parsed"

ior_path = assert ( _G["MORBID_IOR_PATH"] or ...
                  )


oil.main(function()

        --oil.verbose:level(5)
        --oil.verbose.viewer.output = assert(io.open("client.log", "w"))
        --oil.verbose.timed = true
        --local TimeFormat = "%d/%m/%Y %H:%M:%S"
        --oil.verbose:settimeformat(TimeFormat) -- inclui data e hora no log do OiL

        local orb = oil.init()

        local array = require "table"

        --local array = require "table"
        --local unpack = array.unpack or _G.unpack
        --orb.TypeRepository.registry:register(unpack(parsed))
	
	simplest = orb:newproxy(assert(oil.readfrom(ior_path))) -- Get proxy to object
        orb:loadidlfile('/home/felipe/dev/tecgraf/morbid/orb/benchmarks/simplest/simplest.idl')

        print "Starting"
        io.stdout:flush()

        local before = os.clock()	
	for i = 1, 1000000 do
           simplest:foo() 
        end

	print("Finished in ", (os.clock() - before))
        io.stdout:flush()
end)
