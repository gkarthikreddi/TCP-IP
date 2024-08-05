CC=gcc
CFLAGS=-g
TARGET:net.o

OBJS=gluethread/glthread.o \
		  graph.o 		   \
		  net.o
gluethread/glthread.o:gluethread/glthread.c
	${CC} ${CFLAGS} -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o
graph.o:graph.c
	${CC} ${CFLAGS} -c -I . graph.c -o graph.o
net.o:net.c
	${CC} ${CFLAGS} -c -I . net.c -o net.o
clean:
	rm *.o
	rm gluethread/glthread.o
	rm *exe
