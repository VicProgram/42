#!/bin/bash

# Check if argument was passed
if [ $# -eq 0 ]; then
    echo "Error: You need to provide an argument indicating the number of times the message will be displayed"
    exit 1
fi

# Assign the first argument to a variable
times=$1

# Verify that the parameter is a positive integer
if echo "$times" | grep -Eq '^[0-9]+$'; then
    :
else
    echo "Error: The parameter must be a positive integer"
    exit 1
fi

# Loop to show the message
i=0
while [ $i -lt $times ]; do
    echo "This looks good, @alrodri2!"
    i=$((i + 1))
done
