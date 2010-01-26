#!/bin/sh
statusfile=/solar/status.txt
restartscript=/solar/restart.sh
if [ ! -f $statusfile ]; then
  echo Statusfile does not exist: $statusfile;
  exit;
fi
content=`cat $statusfile`
if [ "$content" = "OK" ]; then
  echo Status is OK;
  return;
fi
echo Found Status: $content, restarting
exec $restartscript