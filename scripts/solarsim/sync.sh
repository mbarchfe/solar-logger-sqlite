#!/bin/sh
database_file=db.sqlite3
server_user=user
server_password=password
server_url=http://192.168.1.181:7000
tmp_csv_file=/tmp/csv.txt
inverters="9380933 9380934 9380935 9380936 9380937" 
. ../sync_functions.sh
sync
