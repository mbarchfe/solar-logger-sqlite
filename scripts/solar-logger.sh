#! /bin/sh
PATH=/sbin:/usr/sbin:/bin:/usr/bin
DESC="Solarlogger"
NAME=solar-logger
DAEMON=/usr/bin/$NAME
# SOLARDIR contains solar-logger.ini and so on
SOLARDIR=/var/media/ftp/uStor01/solar
SCRIPTNAME=$SOLARDIR/$NAME.sh
# Exit if the package is not installed
[ -x "$DAEMON" ] || exit 0

#
# Function that starts the daemon/service
#

do_start()
{
	# Return
	#   0 if daemon has been started
	#   1 if daemon was already running
	#   2 if daemon could not be started
	# TODO: do not copy the expression from do_stop
	running=`ps | grep "[s]olar-logger[^.]" | awk '{ print $1 }' | head -1`;
	if [ "$running" != "" ]; then
		echo "Solar-logger is already running";
		return 1;
	fi
	(cd $SOLARDIR; $DAEMON > /dev/null 2>&1 &) || return 2
}

#
# Function that stops the daemon/service
#
do_stop()
{
	# Return
	#   0 if daemon has been stopped
	#   1 if daemon was already stopped
	#   2 if daemon could not be stopped
	#   other if a failure occurred
	running=`ps | grep "[s]olar-logger[^.]" | awk '{ print $1 }' | head -1`;
	if [ "$running" == "" ]; then
		return 0;
	fi
	echo "Killing $running"
	echo $running | xargs -n 1 kill -9
	RETVAL="$?"
	return $RETVAL
	#echo "kill returned " $RETVAL
}

log_daemon_msg () {
    if [ -z "${1:-}" ]; then
        return 1
    fi
    echo "$@"
}

log_end_msg() {
    if [ $1 -eq 0 ]; then
        echo " ...OK."
    elif [ $1 -eq 255 ]; then
        echo " ...warning."
    else
        echo " ...failed!"
    fi
}

case "$1" in
  start)
	[ "$VERBOSE" != no ] && log_daemon_msg "Starting $DESC" "$NAME"
	do_start
	case "$?" in
		0|1) [ "$VERBOSE" != no ] && log_end_msg 0 ;;
		2) [ "$VERBOSE" != no ] && log_end_msg 1 ;;
	esac
	;;
  stop)
	[ "$VERBOSE" != no ] && log_daemon_msg "Stopping $DESC" "$NAME"
	do_stop
	case "$?" in
		0|1) [ "$VERBOSE" != no ] && log_end_msg 0 ;;
		2) [ "$VERBOSE" != no ] && log_end_msg 1 ;;
	esac
	;;
  status)
       status_of_proc "$DAEMON" "$NAME" && exit 0 || exit $?
       ;;
  #reload|force-reload)
	#
	# If do_reload() is not implemented then leave this commented out
	# and leave 'force-reload' as an alias for 'restart'.
  restart|force-reload)
	#
	# If the "reload" option is implemented then remove the
	# 'force-reload' alias
	#
	log_daemon_msg "Restarting $DESC" "$NAME"
	do_stop
	case "$?" in
	  0|1)
		sleep 2
		do_start
		case "$?" in
			0) log_end_msg 0 ;;
			1) log_end_msg 1 ;; # Old process is still running
			*) log_end_msg 1 ;; # Failed to start
		esac
		;;
	  *)
	  	# Failed to stop
		log_end_msg 1
		;;
	esac
	;;
  *)
	echo "Usage: $SCRIPTNAME {start|stop|status|restart|force-reload}" >&2
	exit 3
	;;
esac
