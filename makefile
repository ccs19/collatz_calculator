CC = gcc
CFLAGS = -g -Wall 
OBJECTS = *.c
NAME = mt-collatz
TODELETE = $(NAME) *.o
LIBS = -lpthread -lrt

mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TODELETE)