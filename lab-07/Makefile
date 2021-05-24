
C_SOURCE=$(wildcard *.c)
OBJ=$(C_SOURCE:.c=.o)
PROGRAM=lab07
CFLAGS=-Wall
LDFLAGS=-lpthread

.PHONY: clean all run

all: ${PROGRAM}


${PROGRAM}: ${OBJ}
	gcc -o $@ $^ ${CFLAGS} ${LDFLAGS}

%.o: %.c %.h
	gcc -o $@ $< ${CFLAGS}

run: all
	@./${PROGRAM}

clean:
	rm -rf ${PROGRAM} *.o