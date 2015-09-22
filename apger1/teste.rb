lastEnd = 0
NUMBER_PARTITIONS = 6
size = 500000
actualSize = 0
for i in 0..NUMBER_PARTITIONS-2
    actualSize = size*(i+1)/21;
    puts "begin adress :#{lastEnd} end adress : #{lastEnd+actualSize-1} size : #{(lastEnd+actualSize-1) - (lastEnd)}"
    lastEnd += actualSize
end

puts "begin adress :#{lastEnd} end adress : #{size-1} size : #{size - lastEnd}"
