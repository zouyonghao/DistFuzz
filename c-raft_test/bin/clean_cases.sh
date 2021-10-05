for i in {1..50000}; do
	echo $i
	if ls test_cases/$i; then
		if grep "Address" test_cases/$i/log*; then
			if grep "heap-buffer-overflow" test_cases/$i/log*; then
				if grep "0x81c9b" test_cases/$i/log*; then
					rm -rf test_cases/$i
				fi
			fi
			continue
		fi
		if grep "raft_start():" test_cases/$i/log*; then
			echo "This should be deleted"
			rm -rf test_cases/$i
		fi
	fi
done
