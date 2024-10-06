for i in test_cases/*; do
	echo $i
	if grep "c->flags & REDIS_SUBSCRIBED || c->flags & REDIS_MONITORING" $i/redis*.log; then
		rm -rf $i
	fi
done
