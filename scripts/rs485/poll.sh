#!/bin/sh
while [ 1 ]; do
  echo "Set to 9600 baud"
  stty -F /dev/ttyUSB0 speed 9600 raw
  echo "poll sunmeter"
  cat sunmeter.bin > /dev/ttyUSB0
  echo "done"
  sleep 5
  echo "Set to 1200 baud"
  stty -F /dev/ttyUSB0 speed 1200 raw
  echo "sync online"
  cat inv_synconline_all.bin > /dev/ttyUSB0
  sleep 3
  echo "readspotparams ffff"
  cat inv_readspotparams_ffff.bin > /dev/ttyUSB0
  echo "done"
  sleep 5
done
