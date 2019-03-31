CC = gcc
CFLAGS = -std=c99
SRCS += ep1.c
PROG = ep1.o

all: $(PROG)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(PROG)

clean:
	rm -rf ep1.o
