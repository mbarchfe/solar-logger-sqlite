#!/bin/sh
socat PTY,link=/tmp/mocktty,raw,echo=0 EXEC:"ruby1.9 serialmock.rb"