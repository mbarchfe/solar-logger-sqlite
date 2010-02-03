#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(solarlogger, 0.1, mbarchfe@gmail.com)
AC_CONFIG_SRCDIR([solar-logger.c])
#AM_INIT_AUTOMAKE
#AM_DISABLE_STATIC

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
#AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([modbus/modbus.h sqlite3.h yasdi/libyasdimaster.h ftplib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

AC_CHECK_LIB([sqlite3],[sqlite3_open],[],[
	echo -n "missing sqlite3"
	exit -1
	])

AC_CHECK_LIB(modbus, modbus_init_rtu, [], [
	echo -n "libmodbus not found"
	exit -1
	])

AC_CHECK_LIB(yasdi, yasdiInitialize, [], [
	echo -n "yasdi not found"
	exit -1
	])

AC_CHECK_LIB(yasdimaster, yasdiMasterInitialize, [], [
	echo -n "yasdimaster not found"
	exit -1
	])

AC_CHECK_LIB(ftp, FtpInit, [], [
	echo -n "ftplib not found"
	exit -1
	])

AC_OUTPUT([
        Makefile
])
