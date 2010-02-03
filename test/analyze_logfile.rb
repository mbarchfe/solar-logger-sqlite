require 'iconv'
ic = Iconv.new('UTF-8//IGNORE', 'UTF-8')

@read = []
 
def dump_read
  p @read.join(" ") if @read
  @read = []
end  
   
dump_read();

$stdin.each_line {
  |line|
  vl = ic.iconv(line) 
  if (vl =~ /ttyUSB0: pl2303_read_bulk_callback/) then
    a=vl.split(' ')
    if (a[-4] != "1,") then
      p "Unexpected lenght read: #{a[-4]}"
    end
    @read << a.last
  elsif (vl =~ /send - length/) then
    dump_read()
    p vl
  end
}

dump_read();