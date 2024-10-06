./start_event_control.sh
for i in $(seq 1 $1); do
    echo $i
    mkfifo calc$i.pipe
    sleep infinity > calc$i.pipe &
done
