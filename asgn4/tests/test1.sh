# Run the program with some input
output=$(echo "xxxxx" | ./xd)

# Check the output against the expected result
expected="Expected output"

if [ "$output" == "$expected" ]; then
    echo "test1: PASS"
else
    echo "test1: FAIL"
    echo "Expected: $expected"
    echo "Got: $output"
fi
