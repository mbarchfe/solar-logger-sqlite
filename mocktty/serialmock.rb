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
require 'socket'
serv = TCPServer.new(7070)
begin
       sock = serv.accept_nonblock
rescue Errno::EAGAIN, Errno::ECONNABORTED, Errno::EPROTO, Errno::EINTR
       IO.select([serv])
       retry
end
# replace sock with $stdin if mock tty should be local
sock.each_byte { |b|
 @read << b
 a_ser = serialize(@read)
 f.write(a_ser)
 f.write("\n")
 f.flush
 match = data.select { |p| (false) or ( a_ser =~ Regexp.new(p['request']))}.each {
   |p|
   # replace sock with $stdout if tty should be local
   p['response_a'].each { |h| printf sock, "%c", h }
   #$stdout.flush
   f.write("\nanswered: #{p['name']}\n")
   f.flush();
   @read = []
 }
}
f.close
