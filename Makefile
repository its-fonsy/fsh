CC = cc

LDFLAGS = 
CFLAGS  = 
PROG = fsh

SRC = fsh.c
OBJ = ${SRC:.c=.o}
INC =

all: ${PROG}

${OBJ}: ${INC}

${PROG}: ${OBJ}
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: ${PROG}
	./fsh

clean:
	rm -f ${PROG} ${OBJ}

.PHONY:  all run clean
