# python check-fle-states.py > check_before_stop.log 2>&1
rm log_check_before_stop
timeout 2 curl http://127.0.1.1:8300/raft_stat >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8301/raft_stat >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8302/raft_stat >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8303/raft_stat >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8304/raft_stat >> log_check_before_stop
# ps -ef | grep atomic_server | awk '{print $2}' | xargs sudo pstack >> log_check_before_stop

# if grep "Node stepped down : Raft node receives higher term " log_app*; then
#     echo "catch the stepped down!"
#     sleep 10000
# fi