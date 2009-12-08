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
	if (AnzahlErfassterWR == 0) {
		ausgabe(LOG, DEBUGNORMAL, "Es sind keine Wechselrichter erfasst, überspringe log_sqlite.\n");
		return;
	}
	// TODO: iteriere über AnzahlErfassterWR und nehme nicht nur den ersten
	ausgabe(LOG, DEBUGNORMAL, "Wechselrichter abfragen und in SQLite schreiben.\n");
	time(&start);
	// if successful the result is zero
	result = bind_int_to_insert_statement(stmt, 1, 0);
	// TODO: result = bindChannelValue(1, "Serial Number");
	result = result || bind_int_to_insert_statement(stmt, 2, start);
	result = result || bindChannelValue(3, "Pac");
	result = result || bindChannelValue(4, "E-Total");
	if (result == 0) {
		ausgabe(LOG, DEBUGNORMAL, "Insert statement erfolgreich vorbereitet.\n");
		exec_statement(stmt);
	} else {
		ausgabe(LOG, DEBUGNORMAL, "Es konnten nicht alle Werte an das Insert statement gebunden werden.\n");
	}
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
    if (Ergebnis != 0) {
    	// Bspw timeout
    	ausgabe(LOG, DEBUGNORMAL, "Wert für channel %s konnte nicht ermittelt werden, Fehlercode von GetChannelValue ist %i.\n", channelName, Ergebnis);
    	return Ergebnis;
    }
    ausgabe(LOG, DEBUGNORMAL, "%s: %f\n", channelName, WertDouble);
    return bind_double_to_insert_statement(stmt, index, WertDouble);
}

