
# this will cause the leader election process non-stable
# ./proxy_server 8302 8303 600000   8300 8304 8301 8305 8302 8303 >> log_proxy1 2>&1 &
# ./proxy_server 8304 8300 600000   8304 8300 8305 8301 8303 8302 >> log_proxy2 2>&1 &
# ./proxy_server 8305 8301 600000   8304 8300 8305 8301 8303 8302 >> log_proxy3 2>&1 &
# ./proxy_server 8305 8301 0   8304 8300 8305 8301 8303 8302

../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &