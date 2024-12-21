# specify compiler, compiler flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -ggdb3 -I./include 

# specify location of source code
SRC = ./src

# specify .c files, .o files
CFILES = $(wildcard $(SRC)/*.c)
OBJECTS = $(CFILES:.c=.o)

# specify executable name
BINARY = lex

# rule for creating the executable
all: $(BINARY)

# rule for linking object files
$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# rule for compiling into object files
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

# clean rule.
clean:
	rm -f $(BINARY) $(OBJECTS)

run:
	./lex ./test.txt
