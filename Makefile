CC=gcc
CFLAGS=-g
TARGET:test.exe
LIBS=-lpthread -L ./CommandParser -lcli
OBJS=glthreads/glthread.o 	\
		  graph.o 		   	\
		  utils.o			\
		  net.o				\
		  topologies.o	   	\
		  nwcli.o

test.exe:testapp.o ${OBJS} CommandParser/libcli.a
	${CC} ${CLAGS} testapp.o ${OBJS} -o test.exe ${LIBS}

testapp.o:testapp.c
	${CC} ${CLAGS} -c testapp.c -o testapp.o

glthreads/glthread.o:glthreads/glthread.c
	${CC} ${CLAGS} -c -I glthreads glthreads/glthread.c -o glthreads/glthread.o
graph.o:graph.c
	${CC} ${CLAGS} -c -I . graph.c -o graph.o
utils.o:utils.c
	${CC} ${CLAGS} -c -I . utils.c -o utils.o
topologies.o:topologies.c
	${CC} ${CLAGS} -c -I . topologies.c -o topologies.o
net.o:net.c
	${CC} ${CLAGS} -c -I . net.c -o net.o
nwcli.o:nwcli.c
	${CC} ${CLAGS} -c -I . nwcli.c -o nwcli.o
CommandParser/libcli.a:
	(cd CommandParser; make)
clean:
	rm *.o
	rm glthreads/glthread.o
	rm *exe
	(cd CommandParser; make clean)
all:
	make
	(cd CommandParser; make)
