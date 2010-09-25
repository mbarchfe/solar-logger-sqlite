# Sync data from a local sqlite3 file with a plantwatchdog server
#
# Define the following properties, then source this file and call sync

# The sqlite3 db with the data which should be sync'ed
# database_file

# Credentials and URL of the plantwatchdog server
# server_user,server_password and server_url

# A file to temporarily store the CSV data, e.g /tmp/csv.txt. Make sure this is not on a flash device.
# tmp_csv_file

# A space separated list of inverter ids.
# inverters


rm_tmp_file() {
  if [ -f $tmp_csv_file ]; then
    echo "Deleting $tmp_csv_file"
    rm $tmp_csv_file
  fi
}

exec_sql() {
  sql=$1
  echo "Using database '$database_file', executing sql '$sql'"
  rm_tmp_file;
  # TODO: check for errors from sqlite3, e.g. if a non-existing 
  # database was used.
  echo "$sql" | sqlite3 -csv $database_file > $tmp_csv_file
}

upload() {
  device_id=$1
  file=$2
  if [ ! -s $file ]; then
    echo "$file is empty, not uploading"
    return
  fi;
  url=$server_url/upload/device/$device_id
  echo "Uploading $file to $url"
  result=`curl -u $server_user:$server_password --request PUT $url -T $file`
  echo "Upload result: $result"
}

get_latest_upload_time() {
  device_id=$1
  url=$server_url/latestupload/device/$device_id
  latest_time=`curl -s -S -u $server_user:$server_password $url`
  echo $latest_time
}


sync() {
  latest_time=$(get_latest_upload_time "sunmeter");
  exec_sql "select time, irradiance, temperature from environment where time>$latest_time;"
  upload "sunmeter" $tmp_csv_file

  for inverter in $inverters
  do
    latest_time=$(get_latest_upload_time "$inverter");
    exec_sql "select time, pac, etotal from inverters where time>$latest_time and inverterid=$inverter;"
   upload "$inverter" $tmp_csv_file
  done

  #rm_tmp_file
} 
