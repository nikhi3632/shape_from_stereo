#!/bin/bash

# Initialize a flag to track success
success=true

# Compile the project
make
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    success=false
fi

# Run unit tests
make run-unit-tests
if [ $? -ne 0 ]; then
    echo "Unit tests failed."
    success=false
fi

# Perform checks
make check
if [ $? -ne 0 ]; then
    echo "Checks failed."
    success=false
fi

# Tidy up
make tidy
if [ $? -ne 0 ]; then
    echo "Tidy failed."
    success=false
fi

# Check the overall success
if [ "$success" = true ]; then
    echo "DONE SUCCESS !!!"
else
    echo "RUN FAILED !!!"
    exit 1
fi
