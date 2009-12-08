/*
 sqlite common functionality for sunmeter polling and loqsqlite
 */
#include <sqlite3.h>
#include "sqlite3common.h"
#include "log.h"
#include "yasdi/libyasdimaster.h"

sqlite3* sqlite3db;
char dbname[DBNAMEMAX];


void init_sqlite3common() {
	int retcode;
	TRepository_GetElementStr("Sqlite.path",DEFAULT_DBNAME, dbname, DBNAMEMAX);
	// I found that on the fritzBox the sqlite3 hangs if running multi-threaded:
	// the point is a pthread_join in  testThreadLockingBehavior
	if (sqlite3_threadsafe() == 0) {
		ausgabe(LOG, DEBUGMINI, "SQLite compiled single threaded, good.\n", dbname);
	} else {
		sqlite3_config( SQLITE_CONFIG_SINGLETHREAD );
		ausgabe(LOG, DEBUGMINI, "Set SQLite to single-threaded.\n", dbname);
	}
	ausgabe(LOG, DEBUGMINI, "Connecting to database %s\n", dbname);
	retcode = sqlite3_initialize();
	retcode = sqlite3_open(dbname, &sqlite3db);
	if (retcode != SQLITE_OK) {
		sqlite3_close(sqlite3db);
		ausgabe(LOG, DEBUGMINI, "Error opening database: %s\n", dbname);
		// TODO better to return the status?
		exit(1);
	}
}

void ensure_table(char* createTableStatement) {
	int retcode;
	sqlite3_stmt* stmt;
	retcode = sqlite3_prepare_v2(sqlite3db,
			createTableStatement,
			-1, // If than zero, then stmt is read up to the first nul terminator
			&stmt,
			0); // Pointer to unused portion of stmt
	if (retcode != SQLITE_OK) {
		ausgabe(LOG, DEBUGNORMAL, "Could not prepare for creating table, assuming table exists.\n");
		return;
	}
	retcode = sqlite3_step(stmt);
	if (retcode == SQLITE_DONE) {
		ausgabe(LOG, DEBUGMINI, "Created Table: %s \n", createTableStatement);
	} else {
		ausgabe(LOG, DEBUGMINI, "Unexpected error creating table: %i , statement was '%s'\n", retcode, createTableStatement);
	}
	sqlite3_finalize(stmt);
}


sqlite3_stmt* create_statement(char* statement) {
	int retcode;
	sqlite3_stmt* stmt;
	retcode = sqlite3_prepare_v2(sqlite3db,
			statement,
			-1, // If than zero, then stmt is read up to the first nul terminator
			&stmt,
			0); // Pointer to unused portion of stmt
	if (retcode != SQLITE_OK) {
		ausgabe(LOG, DEBUGMINI, "Could not prepare statement, SQLITE3 error code is %i\n", retcode);
	}
	return stmt;
}

void exec_statement(sqlite3_stmt* stmt) {
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		// TODO print statement, too
		ausgabe(LOG, DEBUGMINI, "Error executing statement.\n");
	}
	sqlite3_reset(stmt);
}


int bind_double_to_insert_statement(sqlite3_stmt* stmt, int index, double value) {
	if (sqlite3_bind_double(stmt, index, value) != SQLITE_OK) {
		ausgabe(LOG, DEBUGMINI, "Could not bind double value at index %i.\n", index);
		return 1;
	}
	return 0;
}

int bind_int_to_insert_statement(sqlite3_stmt* stmt, int index, int value) {
	if (sqlite3_bind_int(stmt, index, value) != SQLITE_OK) {
		ausgabe(LOG, DEBUGMINI, "Could not bind int value at index %i.\n", index);
		return 1;
	}
	return 0;
}

