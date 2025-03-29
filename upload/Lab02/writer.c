#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define PIPE_NAME "darrens_pipe"  // The named pipe

int main() {
    // Open the file for reading (sensor_data.csv)
    // The assignment curiously asks to use two types of opens - fopen() and open()
    // So a little digging what I understand is:
    // * fopen() is high level and open() low level.
    // fopen() works with File STREAMS - so you use fread/fwrite functions. Its buffered.
    // open uses a file descriptor but the key is it DOES NOT buffer, where the fopen does.
    // My best guess here is we need non-buffering to work with pipes because we want the
    // write() to flush immediately so potential clients can pick up immediately.

    printf("Starting writer, dumping csv to pipe: %s\n", PIPE_NAME);
     // Open the CSV file
    FILE *file = fopen("sensor_data.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Check if the pipe exists; if not, create it
    if (access(PIPE_NAME, F_OK) == -1) {
        // Pipe does not exist, create it
        if (mkfifo(PIPE_NAME, 0666) == -1) {
            perror("Error creating pipe");
            fclose(file);
            return EXIT_FAILURE;
        }
    } else {
        // Pipe exists, no need to create it
        printf("Pipe %s already exists, using it...\n", PIPE_NAME);
    }

    // Open the pipe for writing (will block until the reader connects)
    int pipe_fd = open(PIPE_NAME, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Error opening pipe for writing");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Buffer to hold each line read from the file
    char line[256];

    // Read the file line by line and write to the pipe
    while (fgets(line, sizeof(line), file)) {
        // Write the line to the pipe
        if (write(pipe_fd, line, strlen(line)) == -1) {
            perror("Error writing to pipe");
            close(pipe_fd);
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    // Close the file and the pipe
    fclose(file);
    close(pipe_fd);

    printf("Data successfully written to pipe.\n");
    return EXIT_SUCCESS;
}
