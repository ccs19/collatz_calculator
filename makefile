CC = gcc
CFLAGS = -g -Wall 
OBJECTS = *.c
TODELETE = *.o
NAME = mt-collatz
MTHREAD = -lpthread

mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(MTHREAD)

.PHONY: clean
clean:
	rm -f $(NAME) $(TODELETE)