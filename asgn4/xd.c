#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_hex_and_ascii(const unsigned char *buffer, size_t len, unsigned long offset) {
	printf("%08lx: ", offset);
	for (size_t i = 0; i < 16; i += 2) {
		if (i + 1 < len) {
      			printf("%02x%02x ", buffer[i], buffer[i + 1]);
    		} else if (i < len) {
      			printf("%02x   ",
             		buffer[i]);
    		} else{
      			printf("     ");
		}
	}
  	printf(" ");
  	for (size_t i = 0; i < len; i++) {
    		if (buffer[i] >= 32 && buffer[i] <= 126) {
      			printf("%c", buffer[i]);
    		} else {
      			printf(".");
    		}
  	}
  	printf("\n");
}

int main(int argc, char *argv[]) {
	int fd = STDIN_FILENO;
  	if (argc == 2) {
		fd = open(argv[1], O_RDONLY);
    		if (fd < 0) {
			perror("Error opening file");
			return EXIT_FAILURE;
    		}
	}

  	unsigned char buffer[16];
  	ssize_t bytes_read;
  	unsigned long offset = 0;

  	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    		print_hex_and_ascii(buffer, (size_t)bytes_read, offset);
    		offset += (unsigned long) bytes_read;
	}

  	if (fd != STDIN_FILENO) {
    		close(fd);
  	}

  	return EXIT_SUCCESS;
}
