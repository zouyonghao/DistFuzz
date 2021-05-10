DIR=.

SHOULD_CHECK_0=0
SHOULD_CHECK_1=0
SHOULD_CHECK_2=0

for SERVER in 0 1 2; do
    var="SHOULD_CHECK_$SERVER"
    echo "${!var}"
    if ps -ef | grep "data$SERVER" | grep -q "atomic_server"; then
        echo "Server$SERVER exists!"
    else
        eval SHOULD_CHECK_$SERVER=1
    fi
    if ls $DIR/data$SERVER/log/log_in* >/dev/null 2>&1; then
        echo "Server$SERVER has log in progress, should not check it!"
        eval SHOULD_CHECK_$SERVER=1
    fi
    echo "Should check Server$SERVER ? ${!var}"
done

LOG_0=$DIR/data0/log/log_meta
LOG_1=$DIR/data1/log/log_meta
LOG_2=$DIR/data2/log/log_meta
for i in 0,1 0,2, 1,2; do
    IFS=','
    set -- $i
    # echo $1 $2
    var1="SHOULD_CHECK_$1"
    var2="SHOULD_CHECK_$2"
    echo ${!var1} ${!var2}
    if [ ${!var1} == 0 ] && [ ${!var2} == 0 ]; then
        var1="LOG_$1"
        var2="LOG_$2"
        if diff ${!var1} ${!var2}; then
            echo "${!var1} and ${!var2} are same!"
        else
            echo "${!var1} and ${!var2} are not same!"
            exit 1
        fi
    fi
done
