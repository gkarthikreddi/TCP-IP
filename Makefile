CC=gcc
CFLAGS=-g
TARGET:test.exe

OBJS=glthreads/glthread.o \
		  graph.o 		   \
		  utils.o			\
		  net.o				\
		  topologies.o	   

test.exe:testapp.o ${OBJS}
	${CC} ${CLAGS} testapp.o ${OBJS} -o test.exe

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
clean:
	rm *.o
	rm gluethread/glthread.o
	rm *exe
