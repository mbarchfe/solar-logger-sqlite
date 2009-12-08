/*
 * logsqlite.h
 *
 *  Created on: Nov 21, 2009
 *      Author: ubuntu
 */

#ifndef LOGSQLITE_H_
#define LOGSQLITE_H_

#define CREATE_TABLE_LOG_STMT "create table inverters (inverterid integer, time integer, pac double, etotal integer);"
#define INSERT_LOG_STMT "INSERT INTO inverters VALUES (?,?,?,?);"
void log_sqlite();
void init_logsqlite3();
#endif /* LOGSQLITE_H_ */
