../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &
# ../../build/proxy_server/proxy_server ./proxy-config-backup.json > log_proxy 2>&1 &
../../build/event_control/event_control > log_event_control 2>&1 &
