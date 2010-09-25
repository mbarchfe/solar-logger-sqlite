#!/bin/sh
statusfile=/tmp/status.txt
restartscript=/var/media/ftp/uStor01/solar/solar-logger.sh restart
watchdoglog=/tmp/watchdog.log
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
