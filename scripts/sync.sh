#!/bin/sh
database_file=db/db.sqlite3
server_user=user
server_password=password
server_url=http://192.168.0.1
tmp_csv_file=/tmp/csv.txt
inverters="2100147679 2170014725" 
. ./sync_functions.sh
sync
