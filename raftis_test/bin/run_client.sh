GET_A_SERVER_ID=$(ps -ef | grep raftis | grep data | head -n 1 | awk '{print $13}')
if [[ $GET_A_SERVER_ID == '' ]]; then
    echo "no survive server"
    exit -1
fi

echo $GET_A_SERVER_ID

if [[ $1 == 'set' ]]; then
    echo "should set $2"
    if redis-cli -p $GET_A_SERVER_ID set a $2 | grep OK; then
        echo "set success"
        echo $2
    else
        echo "set failed"
        exit -1
    fi
else
    echo "should get"
    VALUE=$(redis-cli -p $GET_A_SERVER_ID get a)
    if echo $VALUE | grep ERR; then
        echo "find ERR!"
        exit -1
    fi
    echo $VALUE
fi
