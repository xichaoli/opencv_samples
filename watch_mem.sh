#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 log_file_name [app_name] [interval]"
    exit 1
fi

# Log file name
LOG_FILE_NAME="$1".csv

# Log file dir
LOG_FILE_DIR="logs"
mkdir -p "${LOG_FILE_DIR}"

# App name
# 将第二个变量的默认值设置为 camera
APP_NAME=${2:-camera}
#echo "App name: ${APP_NAME}"

# Interval
# 将第三个变量的默认值设置为 600
INTERVAL=${3:-600}
#echo "Interval: ${INTERVAL}"

# run app
./build/"${APP_NAME}" &

# wait
sleep 300

# Linux 中 /proc/[pid]/comm 文件中的进程名最多只有 15 个字符
app_pid=$(pgrep "${APP_NAME:0:15}")
ANON_ORIG=$(grep '^Anonymous:' /proc/"${app_pid}"/smaps_rollup | awk '{print $2}')
RSS_ORIG=$(grep '^Rss:' /proc/"${app_pid}"/smaps_rollup | awk '{print $2}')

echo "Watching memory used of name ... Ctrl+C to stop"
echo "Time,Anonymous,RSS" | tee "${LOG_FILE_DIR}"/"${LOG_FILE_NAME}"

SECONDS=0

while true
do
    sleep "${INTERVAL}"
    ANON=$(grep '^Anonymous:' /proc/"${app_pid}"/smaps_rollup | awk '{print $2}')
    RSS=$(grep '^Rss:' /proc/"${app_pid}"/smaps_rollup | awk '{print $2}')
    echo $((SECONDS/3600)):$((SECONDS%3600/60)):$((SECONDS%60)),$((ANON-ANON_ORIG)),$((RSS-RSS_ORIG)) | tee -a "${LOG_FILE_DIR}"/"${LOG_FILE_NAME}"
done
