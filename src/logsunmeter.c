#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <modbus/modbus.h>
#include <unistd.h>
#include <time.h>
#include <modbus/modbus.h>
#include "sqlite3common.h"
#include "logsunmeter.h"
#include "log.h"
#include "yasdi/libyasdimaster.h"

static sqlite3_stmt* stmt;
static modbus_param_t mb_param;
static int modbus_connected = 0;
static int enabled = 0;

void init_logsunmeter() {
  ensure_table(CREATE_SUNMETER_TABLE_LOG_STMT);
  // todo: do we need to call sqlit3_finalize?
  stmt = create_statement(INSERT_SUNMETER_LOG_STMT);
  connectToModbus();
  enabled = TRepository_GetElementInt("Sunmeter.enabled");
  if (enabled) {
    ausgabe(LOG, DEBUGMINI, "Reading from sunmeter is enabled.\n");
  } else {
    ausgabe(LOG, DEBUGMINI, "Reading from sunmeter is disabled\n");
  }
}

int connectToModbus() {
	/* RTU parity : none, even, odd */
	modbus_init_rtu(&mb_param, "/dev/ttyUSB0", TRepository_GetElementInt("Sunmeter.BaudRate", DEFAULT_BAUD_RATE), "none", 8, 1);
	// TODO: also suppress error messages from modbus since Communication Timeout occurs quite frequently
	modbus_set_debug(&mb_param,  TRepository_GetElementInt("Sunmeter.ModbusDebug", FALSE));
}

int readInputParameter(int register_address) {
	uint16_t paramValue;
	int count_read;

	paramValue = 0;
	count_read = read_input_registers(&mb_param, 0x01, // slave
			register_address, // input register address
			1, //count UT_INPUT_REGISTERS_NB_POINTS,
			&paramValue);
	if (count_read != 1) {
		ausgabe(LOG, DEBUGMINI, "Error reading input parameter at register %i.\n", register_address);
		return INPUT_PARAM_READ_ERROR;
	}
	return paramValue;
}

void log_sunmeter_repeatedly() {
	int repeat_count = 0;
	int read_return = log_sunmeter_internal1();
	// TODO: why do many of the first attempts fail?
	while ((read_return != READ_SUCCESSFULLY) && (READ_REPEAT_TRIALS > repeat_count++)) {
		ausgabe(LOG, DEBUGNORMAL, "%i. of %i attempts to read data from sunmeter failed, trying again ...\n", repeat_count, READ_REPEAT_TRIALS);
		sleep(1);
		read_return = log_sunmeter_internal1();
	}
	if (read_return != READ_SUCCESSFULLY) {
		ausgabe(LOG, DEBUGMINI, "Error: Could not read data from sunmeter.\n");
	}
}

int log_sunmeter_internal1() {
	int result;
	int raw_param_value;
	int temperature;
	time_t start;// secs_since_epoch, UTC

	if (mb_param.fd == 0) {
		ausgabe(LOG, DEBUGMINI, "Error: Keine Verbindung zum modbus\n");
		return LOG_ERROR;
	}

	sqlite3_reset(stmt);

	time(&start);
	result = bind_int_to_insert_statement(stmt, 1, start);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: time could not be bound to insert statement\n");
		return LOG_ERROR;
	}
	raw_param_value = readInputParameter(0x0101);
	if (raw_param_value == INPUT_PARAM_READ_ERROR) {
		ausgabe(LOG, DEBUGMINI, "Error: Could not read temperature from sunmeter\n");
		return READ_FAILED;
	}
	// temperature is 2-complement with 14 bits integer and 2 bits fraction
	// the fraction could be considered by adding (or subtracting if negative)
	temperature = raw_param_value>>2;
	// if negative, fill with boolean 1 up to the size of int
	if (temperature & 0x2000) {
		// 0x3fff is 2^14-1
		temperature = (~0x3fff) + temperature;
	}
	ausgabe(LOG, DEBUGNORMAL, " .. temperature: %i\n", temperature);
	result = bind_int_to_insert_statement(stmt, 2, temperature);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: temperature could not be bound to insert statement\n");
		return LOG_ERROR;
	}
	raw_param_value = readInputParameter(0x0100);
	if (raw_param_value == INPUT_PARAM_READ_ERROR) {
		ausgabe(LOG, DEBUGMINI, "Error: Could not read irradiance from sunmeter\n");
		return READ_FAILED;
	}
	ausgabe(LOG, DEBUGNORMAL, " .. irradiance: %i\n", raw_param_value);
	result = bind_int_to_insert_statement(stmt, 3, raw_param_value);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: irradiance could not be bound to insert statement\n");
		return LOG_ERROR;
	}
	exec_statement(stmt);
	return READ_SUCCESSFULLY;
}

void log_sunmeter() {
	if (enabled == 0) {
		return;
	}
	yasdiMasterSetDriverOffline(1);
	ausgabe(LOG, DEBUGNORMAL, "Preparing reading from sunmeter: setting yasdi driver offline\n");
	if (modbus_connect(&mb_param) == -1) {
		ausgabe(LOG, DEBUGMINI, "ERROR: Connection to Modbus failed\n");
	} else {
		ausgabe(LOG, DEBUGNORMAL, "Now connected to Modbus\n");
	}
	log_sunmeter_repeatedly();
	modbus_close(&mb_param);
	ausgabe(LOG, DEBUGNORMAL, "Connection to Modbusc closed\n");
	ausgabe(LOG, DEBUGNORMAL, "Reading from sunmeter finished: setting yasdi driver online\n");
	yasdiMasterSetDriverOnline(1);
}
