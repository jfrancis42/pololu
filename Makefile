CC= cc

#CFLAGS= -g -DDEBUG -I./ -L./
#CFLAGS= -DVERBOSE -I./ -L./
#CFLAGS= -O2 -I./ -L./
CFLAGS= -O2 -I./ -L./ -Wall

all:	libpololu servo 

libpololu:	libpololu.o
		ar r libpololu.a libpololu.o
		ranlib libpololu.a

servo:	servo.o
	$(CC) $(CFLAGS) -o servo servo.o -lpololu

speedo:	speedo.o
	$(CC) $(CFLAGS) -o speedo speedo.o -lpololu -lgps

clean:
	rm -f *.o *.a *~ servo speedo 
