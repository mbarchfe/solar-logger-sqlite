#ifndef SQLITE3COMMON_H_
#define SQLITE3COMMON_H_

#include <sqlite3.h>
// the global sqlite3 connection

//extern sqlite3* sqlite3db;

void init_sqlite3common(char* dbname);
void ensure_table(char* createTableStatement);
sqlite3_stmt* create_statement(char* statement);
void exec_statement(sqlite3_stmt* stmt);
int bind_double_to_insert_statement(sqlite3_stmt* stmt, int index, double value);
int bind_int_to_insert_statement(sqlite3_stmt* stmt, int index, int value);

#endif /* SQLITE3COMMON_H_ */
