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

#define DEFAULT_BAUD_RATE 9600

#define READ_FAILED 0
#define READ_SUCCESSFULLY 1
#define LOG_ERROR 2
#define READ_REPEAT_TRIALS 5

void log_sunmeter();
void init_logsunmeter();

#endif /* LOGSUNMETER_H_ */
