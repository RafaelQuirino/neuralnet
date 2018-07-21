file1 = File.read(ARGV[0])
file2 = File.read(ARGV[1])
file3 = ARGV[2]
printfp = ARGV[3]
printfn = ARGV[4]
#threshold = ARGV[4]

arr1 = file1.split("\n").map{|x| x.to_f}
arr2 = file2.split("\n").map{|x| x.to_i}

acertou = 0
errou = 0
fp = 0
fn = 0
fparr = []
threshold = ARGV[5].to_f
arr1.each_with_index do |item, index|
	msg = ""
	if (item >= threshold and arr2[index] == 1) or
	   (item < threshold and arr2[index] == 0)
		acertou += 1
		msg += "A - "
	else
		errou += 1
		msg += "E - "

		if (arr2[index] == 1)
			fn += 1
			if (printfn == "true")
				line = `sed -n #{index+1}p #{file3}`
				puts "#{index+1} -> #{arr2[index]}, #{arr1[index]}"
				puts "[#{line.gsub("\n","")}]\n"
			end
		else
			fp += 1
			fparr << (index + 1)

			if (printfp == "true")
                line = `sed -n #{index+1}p #{file3}`
                puts "#{index+1} -> #{arr2[index]}, #{arr1[index]}"
                puts "[#{line.gsub("\n","")}]\n"
            end
		end
	end
	#puts "#{msg}#{arr2[index]} - #{item}"
end

puts "Hit\t#{acertou}	#{((acertou.to_f/(acertou + errou))*100).round(4)}%"
puts "Miss\t#{errou}	#{((errou.to_f/(acertou + errou))*100).round(4)}%"
puts "False positives: #{fp}"
puts "False negatives: #{fn}"
#puts fparr.join(" - ")

