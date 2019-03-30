CC = gcc
SRCS += ep1.c
PROG = ep1.o

all: $(PROG)

$(PROG):$(SRCS)
	$(CC) $(SRCS) -o $(PROG)

clean:
	rm -rf ep1.o
