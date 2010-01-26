#!/bin/sh
ps | grep "[s]olar-logger" | awk '{ print $1 }' | xargs -n 1 kill -9
sleep 2
echo "Starte solar-logger as daemon"
# it takes some time after the start of solar-logger
# until the status.txt is written, so we remove the status file here
if [ -f /solar/status.txt ]; then
  rm /solar/status.txt
fi
( cd /solar; /solar/solar-logger > /dev/null 2>&1 & )