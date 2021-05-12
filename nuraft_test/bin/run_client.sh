GET_A_SERVER_ID=$(ps -ef | grep calc_server | grep examples | head -n 1 | awk '{print $9}')
if [[ $GET_A_SERVER_ID == '' ]]; then
    echo "no survive server"
    exit -1
fi

echo st >calc$GET_A_SERVER_ID.pipe
if [[ $1 == 'set' ]]; then
    LEADER_ID=$(tail -n 5 calc$GET_A_SERVER_ID.log | head -n 1 | awk '{print $3}')
    echo "leader is $LEADER_ID"
    if [[ $LEADER_ID == -1 ]]; then
        echo "no leader is found"
        exit -1
    fi
    echo "should set $2"
    echo !$2 >calc$LEADER_ID.pipe
    sleep 0.5
    if tail -n2 calc$LEADER_ID.log | grep succeed; then
        echo "set success"
        echo $2
    else
        echo "set failed"
        exit -1
    fi
else
    echo "should get"
    VALUE=$(tail -n 2 calc$GET_A_SERVER_ID.log | head -n 1 | awk '{print $4}')
    echo $VALUE
fi
