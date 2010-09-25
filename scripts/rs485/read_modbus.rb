f = File.open "/dev/ttyUSB0", "r"
#f = STDIN
f.each_byte {|c| printf "<%x>", c ; STDOUT.flush}
