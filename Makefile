CC = gcc
SRCS += ep1.c
PROG = ep1.o

all: $(PROG) run

$(PROG):$(SRCS)
	$(CC) $(SRCS) -o $(PROG)

run:
	./ep1.o

clean:
	rm -rf ep1.o
