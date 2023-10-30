#!/bin/bash

# Get the PID of ex1.c from the /tmp/ex1.pid file
PID=$(cat /tmp/ex1.pid)

# Extract heap memory address range from /proc/{pid}/maps
HEAP_INFO=$(grep "[ ]*heap" /proc/$PID/maps)
START_ADDR=$(echo $HEAP_INFO | awk '{print $1}' | cut -d'-' -f1)
END_ADDR=$(echo $HEAP_INFO | awk '{print $1}' | cut -d'-' -f2)

# Calculate offset and length for dd
OFFSET=$((0x$START_ADDR))
LENGTH=$((0x$END_ADDR - 0x$START_ADDR))

# Extract the heap memory region using dd, pipe it to xxd, and then grep for the "pass:" pattern. Grab the lines around it to ensure we capture the entire password.
PASSWORD_HEX_DUMP=$(sudo dd if=/proc/$PID/mem bs=1 skip=$OFFSET count=$LENGTH 2>/dev/null | xxd | grep -A 8 "pass:" | head -9)

# Check if PASSWORD_HEX_DUMP is empty or not
if [ -z "$PASSWORD_HEX_DUMP" ]; then
    echo "Password not found in the memory of the process."
    exit 1
fi

# Extract the 3 characters after "pass:"
PASS_VALUE=$(echo "$PASSWORD_HEX_DUMP" | grep -oP '(?<=pass:).{8}')

# Extract the relative address from xxd output
ADDR_HEX=$(echo "$PASSWORD_HEX_DUMP" | head -n 1 | awk '{print $1}' | tr -d ':')
ADDR_DEC=$((16#$ADDR_HEX))

# Calculate the actual memory address
ACTUAL_ADDR=$(printf '%x' $(($OFFSET + $ADDR_DEC)))

# Kill the ex1.c program
sudo kill -SIGKILL $PID

if [ -z "$PASS_VALUE" ]; then
    echo "Password not found in the memory of the process."
else
    echo "Password Found: $PASS_VALUE at address 0x$ACTUAL_ADDR"
fi
