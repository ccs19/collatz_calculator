CC = gcc
CFLAGS = -g -Wall
OBJECTS = mt-collatz.c
TODELETE = mt-collatz.o
NAME = mt-collatz



mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(NAME) $(TODELETE)