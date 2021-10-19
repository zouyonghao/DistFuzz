shopt -s extglob
GET_A_SERVER_ID=$(ps -ef | grep calc_server | grep examples | head -n 1 | awk '{print $9}')
if [[ $GET_A_SERVER_ID == '' ]]; then
    echo "no survive server"
    exit -1
fi

echo "Got a survival server $GET_A_SERVER_ID"
echo st > calc$GET_A_SERVER_ID.pipe
sleep 1
LOG_ID=$(expr $GET_A_SERVER_ID - 1)
if [[ $1 == 'set' ]]; then
    echo "should set"
    echo "LOG_ID is $LOG_ID"
    LEADER_ID=$(tail -n 5 log"$LOG_ID"_* | grep leader | head -n 1 | awk '{print $3}')
    if [ -z "$LEADER_ID" ]; then
        echo "LEADER_ID is empty"
        exit -1
    fi
    echo "leader is $LEADER_ID"
    if [[ $LEADER_ID == -1 ]]; then
        echo "no leader is found"
        exit -1
    fi
    echo "should set $2"
    echo !$2 >calc$LEADER_ID.pipe
    sleep 1

    LOG_ID=$(expr $LEADER_ID - 1)
    if tail -n 2 log"$LOG_ID"_* | grep succeed; then
        echo "set success"
        echo $2
    else
        echo "set failed"
        exit -1
    fi
else
    echo "should get"
    VALUE=$(tail -n 2 log"$LOG_ID"_* | head -n 1 | awk '{print $4}')
    echo $VALUE
fi
