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

static sqlite3_stmt* stmt;
static modbus_param_t mb_param;
static int modbus_connected = 0;
void init_logsunmeter() {
  ensure_table(CREATE_SUNMETER_TABLE_LOG_STMT);
  // todo: do we need to call sqlit3_finalize?
  stmt = create_statement(INSERT_SUNMETER_LOG_STMT);
  connectToModbus();
}

int connectToModbus() {
	/* RTU parity : none, even, odd */
	modbus_init_rtu(&mb_param, "/dev/ttyUSB0", 9600, "none", 8, 1);
	if (modbus_connect(&mb_param) == -1) {
		ausgabe(LOG, DEBUGMINI, "ERROR: Connection to Modbus failed\n");
	}
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
		ausgabe(LOG, DEBUGMINI, "Error reading input parameter at %i.\n", register_address);
		return INPUT_PARAM_READ_ERROR;
	}
	return paramValue;
}

void log_sunmeter() {
	int result;
	int raw_param_value;
	double precise_temperature;
	time_t start;// secs_since_epoch, UTC

	if (mb_param.fd == 0) {
		ausgabe(LOG, DEBUGMINI, "Error: Keine Verbindung zum modbus\n");
		return;
	}

	time(&start);
	result = bind_int_to_insert_statement(stmt, 1, start);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: time could not be bound to insert statement\n");
		return;
	}
	raw_param_value = readInputParameter(0x0101);
	if (raw_param_value == INPUT_PARAM_READ_ERROR) {
		ausgabe(LOG, DEBUGMINI, "Error: Could not read temperature from sunmeter\n");
		return;
	}
	ausgabe(LOG, DEBUGNORMAL, " .. temperature: %i\n", raw_param_value >> 2);
	precise_temperature = (raw_param_value >> 2) + (raw_param_value & 3) * 0.25;
	ausgabe(LOG, DEBUGNORMAL, " .. precise temperature: %f\n", precise_temperature);
	result = bind_int_to_insert_statement(stmt, 2, raw_param_value >> 2);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: temperature could not be bound to insert statement\n");
		return;
	}
	raw_param_value = readInputParameter(0x0100);
	if (raw_param_value == INPUT_PARAM_READ_ERROR) {
		ausgabe(LOG, DEBUGMINI, "Error: Could not read irradiance from sunmeter\n");
		return;
	}
	ausgabe(LOG, DEBUGNORMAL, " .. irradiance: %i\n", raw_param_value);
	result = bind_int_to_insert_statement(stmt, 3, raw_param_value);
	if (result != 0) {
		ausgabe(LOG, DEBUGMINI, "Error: irradiance could not be bound to insert statement\n");
		return;
	}
	exec_statement(stmt);
}
