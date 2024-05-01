#!/bin/bash

# Navigate to the tests directory
cd tests

# Variable to keep track of test failures
failures=0

# Find and run each test script
for test_script in *.sh; do
    echo "Running $test_script..."
    ./"$test_script"
    if [ $? -ne 0 ]; then
        ((failures++))
    fi
    echo ""
done

# Return to the original directory
cd ..

# Summary
if [ $failures -eq 0 ]; then
    echo "All tests passed."
else
    echo "$failures test(s) failed."
    exit 1
fi

