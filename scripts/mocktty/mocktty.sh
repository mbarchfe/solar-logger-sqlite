#!/bin/sh
# On the server change directory to solar-logger-sqlite/mocktty
# and start ruby serialmock.rb
server=192.168.1.181
socat PTY,link=/tmp/mocktty TCP:$server:7070

