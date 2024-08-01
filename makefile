all: 
	gcc -o mapper ./src/*.c -Wall -std=c99 -ggdb3 -I./include/
