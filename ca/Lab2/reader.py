import os

# Define the name of the pipe
PIPE_NAME = "darrens_pipe"  # Using the same pipe name as my writer.


# Block here will check if the pipe exists, if not creates it.
if not os.path.exists(PIPE_NAME):
    print("Pipe {} does not exist. Creating pipe...".format(PIPE_NAME))

    # Create the named pipe
    mkfifo_result = os.mkfifo(PIPE_NAME)  # Create the named pipe

    if mkfifo_result != 0:
        print("Error creating pipe {}. Exiting program.".format(PIPE_NAME))
        exit(1)  # Exit if pipe creation fails
    else:
        print("Pipe {} created successfully.".format(PIPE_NAME))

# Open the pipe for reading
with open(PIPE_NAME, 'r') as fifo:
    print("Reading from pipe: {}...".format(PIPE_NAME))

    # Loop and read/print each line from the pipe
    while True:
        line = fifo.readline()  # Read one line at a time
        if not line:  # We exit out if there is no more data.
            print("No more data in the pipe.")
            break
        print(f"Received: {line.strip()}")  # Print the received line, clean it up wiht leading spaces.

print("Finished reading from pipe.")
