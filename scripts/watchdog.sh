#!/bin/sh
statusfile=/solar/status.txt
restartscript=/solar/restart.sh
watchdoglog=/solar/watchdog.log
if [ ! -f $statusfile ]; then                 
  echo `date`: Statusfile does not exist: $statusfile >> $watchdoglog
  exit;
fi                       
content=`cat $statusfile`     
if [ "$content" = "OK" ]; then    
  touch $watchdoglog        
  return;
fi                                                             
echo `date`: Found Status: $content, restarting >> $watchdoglog
exec $restartscript
