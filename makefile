CC = gcc
CFLAGS = -g -Wall 
OBJECTS = mt-collatz.c
TODELETE = mt-collatz.o
NAME = mt-collatz
MTHREAD = -lpthread



mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(MTHREAD)

.PHONY: clean
clean:
	rm -f $(NAME) $(TODELETE)