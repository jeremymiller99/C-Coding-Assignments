CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic

# List your .c files here
SOURCES = bitreader.c bitwriter.c node.c pq.c huff.c dehuff.c \
          brtest.c bwtest.c nodetest.c pqtest.c
# List your header files here
HEADERS = bitreader.h bitwriter.h node.h pq.h

# Define your executables
EXECUTABLES = huff dehuff brtest bwtest nodetest pqtest

# Default target
all: $(EXECUTABLES)

# Individual targets for executables
huff: huff.c bitwriter.c node.c pq.c
	$(CC) $(CFLAGS) huff.c bitwriter.c node.c pq.c -o huff

dehuff: dehuff.c bitreader.c node.c pq.c
	$(CC) $(CFLAGS) dehuff.c bitreader.c node.c pq.c -o dehuff

brtest: brtest.c bitreader.c
	$(CC) $(CFLAGS) brtest.c bitreader.c -o brtest

bwtest: bwtest.c bitwriter.c
	$(CC) $(CFLAGS) bwtest.c bitwriter.c -o bwtest

nodetest: nodetest.c node.c
	$(CC) $(CFLAGS) nodetest.c node.c -o nodetest

pqtest: pqtest.c pq.c node.c
	$(CC) $(CFLAGS) pqtest.c pq.c node.c -o pqtest

# Format all C and header files
format:
	clang-format -i $(SOURCES) $(HEADERS)

# Clean up build artifacts
clean:
	rm -f $(EXECUTABLES) *.o

