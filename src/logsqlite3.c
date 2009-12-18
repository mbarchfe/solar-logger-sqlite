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
	char  deviceName[DEVICE_NAME_LENGTH]={0};
    DWORD serialNumber;
	int result;
	time_t start;
	int wri;

	if (AnzahlErfassterWR == 0) {
		ausgabe(LOG, DEBUGNORMAL, "Es sind keine Wechselrichter erfasst, überspringe log_sqlite.\n");
		return;
	}
	for(wri=0;wri<AnzahlErfassterWR;wri++) {
		sqlite3_reset(stmt);
		GetDeviceName( WRHandles[wri], deviceName, DEVICE_NAME_LENGTH);
		ausgabe(LOG, DEBUGNORMAL, "Wechselrichter '%s' abfragen und in SQLite schreiben.\n", deviceName);
		result = GetDeviceSN( WRHandles[wri], &serialNumber );
		if (result != 0) {
			ausgabe(LOG, DEBUGMINI, "Seriennummer konnte nicht ermittelt werden für %s", deviceName);
			continue;
		}
		time(&start);
		// if successful the result is zero
		result = bind_int_to_insert_statement(stmt, 1, serialNumber);
		result = result || bind_int_to_insert_statement(stmt, 2, start);
		result = result || bindChannelValue(wri, 3, "Pac");
		result = result || bindChannelValue(wri, 4, "E-Total");
		if (result == 0) {
			ausgabe(LOG, DEBUGNORMAL, "Insert statement erfolgreich vorbereitet.\n");
			exec_statement(stmt);
		} else {
			ausgabe(LOG, DEBUGNORMAL, "Es konnten nicht alle Werte an das Insert statement gebunden werden.\n");
		}
	}
}


int bindChannelValue(int wrIndex, int stmtIndex, char* channelName) {
	char		WertText[50]={0};
	int Zeichen = sizeof(WertText)-1;
	double	WertDouble = -1.0;
	int Ergebnis;

	DWORD KanalHandle = FindChannelName(WRHandles[wrIndex], channelName);
	/* max alter: wert groesser null, denn yasdi bekommt immer mehrere Spotwerte
	 * bei einer modbus abfrage. Werden mehrere Spotwerte hintereinander abgefragt,
	 * dann können diese Werte aus einer physikalischen Abfrage verwendet werden
	 * Ein Wert von 0 würde mehrere physikalische Abfragen nach sich ziehen.
	 */
    Ergebnis = GetChannelValue( KanalHandle,
								WRHandles[wrIndex],
								&WertDouble,
								0,
                                0,
                                3 /* max alter in secs */);
    if (Ergebnis != 0) {
    	// Bspw timeout
    	ausgabe(LOG, DEBUGNORMAL, "Wert für channel %s konnte nicht ermittelt werden, Fehlercode von GetChannelValue ist %i.\n", channelName, Ergebnis);
    	return Ergebnis;
    }
    ausgabe(LOG, DEBUGNORMAL, "%s: %f\n", channelName, WertDouble);
    return bind_double_to_insert_statement(stmt, stmtIndex, WertDouble);
}

