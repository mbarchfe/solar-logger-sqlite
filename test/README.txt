This directory contains scripts which provide a mock tty (pseudo tty) which emulates
the behaviour of a RS 485 line with one inverter and one sunmeter attached.

Calling mocktty.sh creates a mock tty at /tmp/mocktty. The ruby script serialmock.rb matches
the data written to /tmp/mocktty with request patterns given in mockdata.yaml. If a valid
pattern is found, the script writes the appropriate response to /tmp/mocktty.

With the solar-logger.ini set up accordingly the mock tty should allow to run the
solar-logger without serial devices attached. You should set at least the following attributes 
in solar-logger.ini:

[DriverModules]
Driver0=libyasdi_drv_serial;

[COM1]
Device=/tmp/mocktty;
Media=RS485;
Baudrate=1200;

[Sunmeter]
enabled=1
ModbusDebug=1

[WR]
Anzahl=1;
ErfassungsIntervall=0;

If you have an inverter connected and want to sniff the communication and add it to mockdata.yaml
have a look at analyze_logfile.rb.

Check /tmp/serialmock.log for the status of the emulation and to see received and sent data.