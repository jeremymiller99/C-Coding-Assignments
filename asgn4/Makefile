CC=gcc
CFLAGS=-Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic

# The name of your executable
TARGET=xd

# Default rule to build the program
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

# Rule to compile the object file
$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

# Rule to clean up build artifacts
clean:
	rm -f $(TARGET) $(TARGET).o

# Rule for formatting source code (assuming clang-format is used)
format:
	clang-format -i $(TARGET).c
