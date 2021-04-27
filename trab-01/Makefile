C_SOURCE=$(wildcard *.c)
OBJ=$(C_SOURCE:.c=.o)
PROGRAM=quicksort
CFLAGS=-Wall
LDFLAGS=-lpthread

.PHONY: clean all

all: ${PROGRAM}

${PROGRAM}: ${OBJ}
	gcc -o $@ $^ ${CFLAGS} ${LDFLAGS}

%.o: %.c %.h
	gcc -o $@ $< ${CFLAGS}

clean:
	rm -rf ${PROGRAM} *.o