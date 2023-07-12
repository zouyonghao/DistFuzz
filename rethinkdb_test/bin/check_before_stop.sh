rm log_check_before_stop
timeout 2 curl http://127.0.1.1:8080 >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8081 >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8082 >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8083 >> log_check_before_stop
timeout 2 curl http://127.0.1.1:8084 >> log_check_before_stop
