#include <sqlite3.h>
#include "sqlite3common.h"
#include "logsqlite3.h"
#include "solar-logger.h"
#include "log.h"
#include "wrerfassen.h"

static sqlite3_stmt* stmt;

void init_logsqlite3() {
  ensure_table(CREATE_TABLE_LOG_STMT);
  // todo: do we need to call sqlit3_finalize?
  stmt = create_statement(INSERT_LOG_STMT);
}

void log_sqlite() {
	int result;
	time_t start;
	ausgabe(LOG, DEBUGNORMAL, "Wechselrichter abfragen und in SQLite schreiben\n");
	time(&start);
	bind_int_to_insert_statement(stmt, 1, 0);
	bind_int_to_insert_statement(stmt, 2, start);
	bindChannelValue(3, "Uac");
	bindChannelValue(4, "Iac");
	exec_statement(stmt);
}


int bindChannelValue(int index, char* channelName) {
	char		WertText[50]={0};
	int Zeichen = sizeof(WertText)-1;
	double	WertDouble = 0;
	int Ergebnis;
	DWORD KanalHandle = FindChannelName(WRHandles[0], channelName);
	/* max alter: wert groesser null, denn yasdi bekommt immer mehrere Spotwerte
	 * bei einer modbus abfrage. Werden mehrere Spotwerte hintereinander abgefragt,
	 * dann können diese Werte aus einer physikalischen Abfrage verwendet werden
	 * Ein Wert von 0 würde mehrere physikalische Abfragen nach sich ziehen.
	 */
    Ergebnis = GetChannelValue( KanalHandle,
								WRHandles[0],
								&WertDouble,
								WertText,
                                Zeichen,
                                3 /* max alter in secs */);
    ausgabe(LOG, DEBUGNORMAL, "%s: %f\n", channelName, WertDouble);
    return bind_double_to_insert_statement(stmt, index, WertDouble);
}

