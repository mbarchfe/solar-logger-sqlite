f= File.new "/tmp/modbus.bin", "w"
#request=[0x01, 0x04 ,0x01, 0x01, 0x00, 0x01, 0x61, 0xF6 ]
#request="7e ff 03 40 41 00 00 ff ff 00 00 0b 0f 09 00 7d 32 60 7e"
#request="7e ff 03 40 41 00 00 f1 ff 00 00 0b 0f 09 00 1f 50 7e"
#request="7e ff 03 40 41 00 00 00 00 80 00 06 d7 c0 7e"
request="7e ff 03 40 41 00 00 00 00 80 00 0a 5b df 31 4b 4b ad 7e"
# setze netwerkadresse ffff for inverter
#request="7e ff 03 40 41 00 00 00 00 80 00 03 d5 b5 2d 7d 5d ff ff 1e b7"
#request ="7e ff 03 40 41 00 00 00 00 80 00 03 03 b6 2d 7d 5d f1 ff fb 6d"
numbers=request.split(" ").collect{|s| h="0x"+s;h.hex}
numbers.each { |h| printf f, "%c", h }
f.close
