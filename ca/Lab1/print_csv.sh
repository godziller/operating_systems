#!/usr/bin/bash

# Take the filename from command-line argument
input_file="$1"

# Check if the file exists
# This is part 2 of the assignment.
if [ ! -e "$input_file" ]; then
    echo "File not found: $input_file"
    exit 1
fi

# Read the file line by line and output in the desired format
# Part 3 of the assignment - simple loop, dumping out the data.
while IFS=',' read -r id timestamp temp pressure; do
    echo "ID : $id , Time : $timestamp , Temp : $temp , Pressure : $pressure"
done < "$input_file"
