CC=clang
CFLAGS=-Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
OBJS=graph.o path.o stack.o tsp.o
HEAD=graph.h path.h stack.h
EXEC=tsp

.PHONY: clean format scan-build

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) *.o

scan-build: clean
	scan-build --use-cc=clang make

format:
	clang-format -i -style=file *.[ch]

