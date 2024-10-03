grep "Unexpected exception, exiting abnormally" test_cases/*/log_app_0_0 | awk '{split($0, a, ":"); print a[1];}' | xargs grep -L "Unable to load database"
