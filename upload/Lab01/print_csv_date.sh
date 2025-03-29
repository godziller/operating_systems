#!/bin/bash

# Takes the filename as an argument
input_file="$1"

# Check if the input file exists
if [ ! -e "$input_file" ]; then
    echo "Error: Input file not found: $input_file"
    exit 1
fi

# Read the file line by line (skip the header if it exists)
# tail -n +2 basically rips off the first line of the file - the header.
tail -n +2 "$input_file" | while IFS=',' read -r id timestamp temp pressure; do
    # Convert the Unix timestamp to make it easier to read
    # Use `date` to format the timestamp as the assignment suggests.
    formatted_time=$(date -d @"$timestamp" "+%Y-%m-%d %H:%M:%S")

    # Print the data in the required format
    echo "ID : $id , Time : $formatted_time , Temp : $temp , Pressure : $pressure"
done
