#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 log_file_name [app_name] [interval]"
    exit 1
fi

# Log file name
LOG_FILE_NAME="$1".csv

# App name
# 将第二个变量的默认值设置为 camera
APP_NAME=${2:-camera}
#echo "App name: ${APP_NAME}"

# Interval
# 将第三个变量的默认值设置为 600
INTERVAL=${3:-600}
#echo "Interval: ${INTERVAL}"

echo "Watching memory used of name ... Ctrl+C to stop"
echo "Time,%MEM,USS,PSS,RSS" | tee "${LOG_FILE_NAME}"

while true
do
    #output=$(smem -H -P camera -c "uss pss rss" | awk 'END {print $1","$2","$3}')
    app_pid=$(pgrep "${APP_NAME}")
    output=$(ps -o %mem,uss,pss,rss "${app_pid}" | awk 'END {print $1","$2","$3","$4}')
    echo "$(date +%H:%M:%S),${output}" | tee -a "${LOG_FILE_NAME}"
    sleep "$INTERVAL"
done
