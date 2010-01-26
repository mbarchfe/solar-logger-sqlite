#!/bin/sh
ps | grep "[s]olar-logger" | awk '{ print $1 }' | xargs -n 1 kill -9
sleep 2
echo "Starte solar-logger as daemon"
( cd /solar; /solar/solar-logger > /dev/null 2>&1 & )