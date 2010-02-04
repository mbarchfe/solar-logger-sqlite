# this ruby script analyzes log files written from the pl2303 driver
# and creates a format suitable as input for serialmock.rb
# the pl2303 usb driver must be started in debug mode with
# modprobe pl2303 debug=1
# log output can usually be found in /var/log/messages
# on a fritz box you must probably also start the syslog:
# syslogd -s 0
# klogd
require 'iconv'
ic = Iconv.new('UTF-8//IGNORE', 'UTF-8')

@read = []
 
def dump_read
  printf " 'response': '#{@read.join(" ")}'\n" if @read
  printf "},\n"
  @read = []
end  
   
dump_read();

$stdin.each_line {
  |line|
  vl = ic.iconv(line) 
  if (vl =~ /ttyUSB0: pl2303_read_bulk_callback/) then
    a=vl.split(' ')
    if (a[-4] != "1,") then
      p "Unexpected length read: #{a[-4]}"
    end
    @read << a.last
  elsif (vl =~ /send - length/) then
    dump_read()
    printf $stdout, "{\n"
    printf $stdout,  " 'name':  'cmd',\n"
    printf $stdout,  " 'request':  '#{vl.split("data = ").last.chomp}',\n"
  end
}

dump_read();