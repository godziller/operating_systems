#!/usr/bin/bash

input_file="./sensor_data.csv"
output_file="./filtered_sensor_data.csv"


if [ -e "$input_file" ]; then
	echo "File exists: $input_file"
else
	echo "File not found: $input_file"
fi 

#checking output files existence

if [ -e "$output_file" ]; then
	echo "File exists: $output_file"
else
	echo "File not found: $output_file"
fi

#testing if output file exists as a regular file:

if [ -f "$output_file[@]" ]; then
	echo "File ($output_file) is a regular file"
else
	echo "it is NOT REGULAR!!"
fi

# Question 1 method
while IFS=' ,:' read -r id timestamp temp pressure; do
    # Debugging: Print out the raw line and timestamp
    echo "Processing line: ID=$id, Timestamp=$timestamp, Temp=$temp, Pressure=$pressure"

    # Check if timestamp is empty or invalid
    if [ -z "$timestamp" ] || [ ! "$timestamp" =~ ^[0-9]+$ ]; then
        echo "Invalid timestamp: $timestamp for ID: $id"
        continue
    fi

    # Convert the Unix timestamp to the desired format with spaces
    time=$(date -d @$timestamp +"%Y -%m -%d %H:%M:%S")

    # Output the formatted result
    echo -e "ID : $id , Time : $time , Temp : $temp , Pressure :\n$pressure"
done < "$input_file"
