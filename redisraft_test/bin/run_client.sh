GET_A_SERVER_ID=$(ps -ef | grep redis | grep -v grep | grep -v strace | grep redis-server | head -n 1 | awk '{print substr($9, 11)}')
if [[ $GET_A_SERVER_ID == '' ]]; then
    echo "no survive server"
    exit -1
fi

# ps -ef | grep redis | grep -v grep | grep redis-server | head -n 1
# echo $GET_A_SERVER_ID

LEADER_ID=

MESSAGE=$(redis-cli -h 127.0.1.1 -p $GET_A_SERVER_ID get a)
echo "MESSAGE is $MESSAGE"
if echo $MESSAGE | grep CLUSTERDOWN; then
    echo "CLUSTERDOWN"
    exit -1
elif echo $MESSAGE | grep MOVED; then
    LEADER_ID=$(echo $MESSAGE | awk '{print substr($3, 11)}')
else
    LEADER_ID=$GET_A_SERVER_ID
fi

echo "LEADER_ID is $LEADER_ID"

if [[ $1 == 'set' ]]; then
    echo "should set $2"
    if redis-cli -h 127.0.1.1 -p $LEADER_ID set a $2 | grep OK; then
        echo "set success"
        echo $2
    else
        echo "set failed"
        exit -1
    fi
else
    echo "should get"
    VALUE=$(redis-cli -h 127.0.1.1 -p $LEADER_ID get a)
    echo $VALUE
fi
