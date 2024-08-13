CC=gcc
CFLAGS=-g -Wall
INCLUDES=-I .
CLILIB=libcli.a
TARGET:exe ${CLILIB}
OBJ=cmd_hier.o parser.o serialize.o 
exe:testapp.o ${CLILIB}
	@echo "Building final executable"
	@ ${CC} ${CFLAGS} ${INCLUDES} testapp.o -o exe -L . -lcli -lpthread 
cmd_hier.o:cmd_hier.c
	@echo "Building cmd_hier.o"
	@ ${CC} ${CFLAGS} -c ${INCLUDES} cmd_hier.c -o cmd_hier.o
parser.o:parser.c
	@echo "Building parser.o"
	@ ${CC} ${CFLAGS} -c ${INCLUDES} parser.c -o parser.o
serialize.o:serialize.c
	@echo "Building serialize.o"
	@ ${CC} ${CFLAGS} -c ${INCLUDES} serialize.c -o serialize.o
testapp.o:testapp.c
	@echo "Building testapp.o"
	@ ${CC} ${CFLAGS} -c ${INCLUDES} testapp.c -o testapp.o
${CLILIB}: ${OBJ}
	@echo "Building Library ${CLILIB}"
	ar rs ${CLILIB} ${OBJ}
clean:
	rm -f exe
	rm -f *.o
	rm -f ${CLILIB}
