#!/bin/bash

# Takes the input filename as the first argument and output filename as the second argument
input_file="$1"
output_file="$2"

# Check if the input file exists
if [ ! -e "$input_file" ]; then
    echo "Error: Input file not found: $input_file"
    exit 1
fi

# Check if the output filename is provided. Now the assignment asks for a second argument, an output file target.
if [ -z "$output_file" ]; then
    echo "Error: Output filename is missing."
    exit 1
fi

# Check if the output file already exists
if [ -e "$output_file" ]; then
    echo "Error: Output file already exists: $output_file"
    exit 1
fi

# so this time we a) remove header, b) Sort using unix time column,
# c) remove junk time then d) enter out while loop
# so we are passing a batton here using the pipe feature.
# tail -> sort -> awk -> loop/nice time/ dump to outputfile.
tail -n +2 "$input_file" | sort -t',' -k2,2n | awk -F',' '$3 <= 50' | while IFS=',' read -r id timestamp temp pressure; do
    # Write the valid line to the output file
    # Convert the Unix timestamp to make it easier to read
    # Use `date` to format the timestamp as the assignement suggests.
    formatted_time=$(date -d @"$timestamp" "+%Y-%m-%d %H:%M:%S")

    # Print the data in the required format
    echo "ID : $id , Time : $formatted_time , Temp : $temp , Pressure : $pressure" >> "$output_file"
done

echo "Filtered data saved to: $output_file"
