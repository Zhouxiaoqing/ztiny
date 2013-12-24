.PHONY: clean

CC = gcc
CFLAGS = -g -Wall
LFLAGS = 

OBJS = main.o lex.o util.o

twt : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o twt

main.o : global.h lex.h main.c
	${CC} ${CFLAGS} -c main.c

lex.o : global.h lex.h lex.c
	${CC} ${CFLAGS} -c lex.c

util.o : global.h util.h util.c
	${CC} ${CFLAGS} -c util.c

clean :
	-rm -rf *.o
	-rm -rf twt
