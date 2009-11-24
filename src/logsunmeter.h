/*
 * logsunmeter.h
 *
 *  Created on: Nov 22, 2009
 *      Author: ubuntu
 */

#ifndef LOGSUNMETER_H_
#define LOGSUNMETER_H_

#define CREATE_SUNMETER_TABLE_LOG_STMT "create table environment (time integer, temperature integer, irradiance double);"
#define INSERT_SUNMETER_LOG_STMT "insert into environment values (?, ?, ?);"
#define INPUT_PARAM_READ_ERROR 1000
void log_sunmeter();
void init_logsunmeter();

#endif /* LOGSUNMETER_H_ */
