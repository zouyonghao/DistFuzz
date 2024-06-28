ps -ef | grep namenode2 | grep -v grep | grep -v bin/rr | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
exit 0