
#require 'pty'
#stdin, stdout, childpid = PTY.spawn("/bin/csh")

#require 'open3'

#stdin, stdout, stderr = Open3.popen3('/bin/sh < /dev/ptmx')
# select( [$stdin], nil, nil, 1.5 )
require 'yaml'

def conv_hex(a)
  a.split(" ").collect{|s| h="0x"+s;h.hex}
end
  
def serialize(a)
  a.collect {|n| sprintf "%02x",n}.join(" ")
end
data = YAML.load_file("mockdata.yaml")
data.each { |r| r['request_a']=conv_hex r['request'];r['response_a']=conv_hex r['response']}
  
f=File.open("/tmp/serialmock.log", "w")
f.write("Serial Mock started with configuration:\n")
f.write(data.to_s)
f.write("\n")
f.flush

@read = []

$stdin.each_byte { |b|
 #printf f, "%0x ",b
 #f.flush
 @read << b
 a_ser = serialize(@read)
 f.write(a_ser)
 f.write("\n")
 f.flush
 match = data.select { |p| (false) or ( a_ser =~ Regexp.new(p['request']))}.each {
   |p|
   p['response_a'].each { |h| printf $stdout, "%c", h }
   $stdout.flush
   f.write("\nanswered: #{p['name']}\n")
   f.flush();
   @read = []
 }
}
f.close