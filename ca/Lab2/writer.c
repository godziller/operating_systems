# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <string.h>

int main () {
	// printf (" I am an empty c program !\ n ") ;
	// open file
	FILE* file = fopen("sensor_data.csv", "r");
	
	// Create Pipe
	
	// Create pointer representing FIFO file that 	
	const char *fifo_path = "my_fifo";

	if (mkfifo(fifo_path, 0666) == -1){
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}


	// Setting FIFO file to read only
	int fd = open(fifo_path, O_RDONLY);
	if (fd == -1){
		perror("open");
		exit(EXIT_FAILURE);
	}

	// set buffer size for each line
	char line[256];
	ssize_t bytes_read;
	while ((bytes_read = read(fd, line, sizeof(line) -1)) > 0){
		line[bytes_read] = '\0';
		printf("%s", line);
	}
	
	close(fd);
	unlink(fifo_path);
	return EXIT_SUCCESS;
	return 0;
}
