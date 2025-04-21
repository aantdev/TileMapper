# specify compiler, compiler flags
CC = gcc


# specify location of source code
SRC_ = ./src


INC_DIRS = $(dir $(shell find ./src -name "*.h"))
INCLUDES = $(addprefix -I,$(INC_DIRS))

# specify .c files, .o files
CFILES = $(shell find ./src -name "*.c")
OBJECTS = $(CFILES:.c=.o)

# flags for compilation
CFLAGS = -std=c99 -Wall -Wextra -ggdb3 $(INCLUDES)

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
