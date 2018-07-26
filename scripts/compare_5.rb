class String
	def black;          "\e[30m#{self}\e[0m" end
	def red;            "\e[31m#{self}\e[0m" end
	def green;          "\e[32m#{self}\e[0m" end
	def brown;          "\e[33m#{self}\e[0m" end
	def blue;           "\e[34m#{self}\e[0m" end
	def magenta;        "\e[35m#{self}\e[0m" end
	def cyan;           "\e[36m#{self}\e[0m" end
	def gray;           "\e[37m#{self}\e[0m" end
	
	def bg_black;       "\e[40m#{self}\e[0m" end
	def bg_red;         "\e[41m#{self}\e[0m" end
	def bg_green;       "\e[42m#{self}\e[0m" end
	def bg_brown;       "\e[43m#{self}\e[0m" end
	def bg_blue;        "\e[44m#{self}\e[0m" end
	def bg_magenta;     "\e[45m#{self}\e[0m" end
	def bg_cyan;        "\e[46m#{self}\e[0m" end
	def bg_gray;        "\e[47m#{self}\e[0m" end
	
	def bold;           "\e[1m#{self}\e[22m" end
	def italic;         "\e[3m#{self}\e[23m" end
	def underline;      "\e[4m#{self}\e[24m" end
	def blink;          "\e[5m#{self}\e[25m" end
	def reverse_color;  "\e[7m#{self}\e[27m" end
end

file1 = File.read(ARGV[0])
file2 = File.read(ARGV[1])
file3 = ARGV[2]
printfp = ARGV[3]
printfn = ARGV[4]
#threshold = ARGV[4]

#arr1 = file1.split("\n").map{|x| x.to_f}
arr1 = file1.split("\n")
arr1.each_with_index do |item,index|
	arr1[index] = item.split(" ").map{|x| x.to_f}
end

arr2 = file2.split("\n").map{|x| x.to_i}

acertou = 0
errou = 0
fp = 0
fn = 0
fparr = []
threshold = ARGV[5].to_f

arr1.each_with_index do |item, index|
	# puts "[#{item[0]},#{item[1]}] => #{item[1]-item[0]}"
	# next

	# condition = arr1[index][0] > arr1[index][1]
	# condition = (arr1[index][1] - arr1[index][0]) < 0.01
	condition = true

	if (
		(arr1[index][0] >= threshold and arr2[index] == 1) or
		(arr1[index][0] <  threshold and arr2[index] == 0)
		)

		acertou += 1

	else
		if arr2[index] == 0 and arr1[index][0] >= threshold and !condition
			acertou += 1
		else
			
			errou += 1

			if (arr2[index] == 1)
				fn += 1
				if (printfn == "true")
					line = `sed -n #{index+1}p #{file3}`
					puts "#{index+1} -> #{arr2[index]}, [#{arr1[index][0]},#{arr1[index][1]}]"
					begin
						puts "[#{line.gsub("\n","")[0..159]}]"
						puts "[#{line.gsub("\n","")[160..319]}]"
						puts "[#{line.gsub("\n","")[320..479]}]"
						puts "[#{line.gsub("\n","")[480..639]}]".green.bold
						puts "[#{line.gsub("\n","")[640..799]}]"
						puts "[#{line.gsub("\n","")[800..959]}]"
						puts "[#{line.gsub("\n","")[960...1120]}]\n\n"
						rescue 
					end
				end
			else
				fp += 1
				fparr << (index + 1)

				if (printfp == "true")
					line = `sed -n #{index+1}p #{file3}`
					puts "#{index+1} -> #{arr2[index]}, [#{arr1[index][0]},#{arr1[index][1]}]"
					begin
						puts "[#{line.gsub("\n","")[0..159]}]"
						puts "[#{line.gsub("\n","")[160..319]}]"
						puts "[#{line.gsub("\n","")[320..479]}]"
						puts "[#{line.gsub("\n","")[480..639]}]".green.bold
						puts "[#{line.gsub("\n","")[640..799]}]"
						puts "[#{line.gsub("\n","")[800..959]}]"
						puts "[#{line.gsub("\n","")[960...1120]}]\n\n"
						rescue
					end
				end
			end
		end
	end
end

puts "Hit\t#{acertou}	#{((acertou.to_f/(acertou + errou))*100).round(4)}%"
puts "Miss\t#{errou}	#{((errou.to_f/(acertou + errou))*100).round(4)}%"
puts "False positives: #{fp}"
puts "False negatives: #{fn}"

