CC= cc

#CFLAGS= -g -DDEBUG -I./ -L./
#CFLAGS= -DVERBOSE -I./ -L./
#CFLAGS= -O2 -I./ -L./
CFLAGS= -O2 -I./ -L./ -Wall -fPIC

all:	libpololu servo 

libpololu:	libpololu.o
		ar r libpololu.a libpololu.o
		ranlib libpololu.a
		gcc -shared -fPIC -Wl,-soname,libpololu.so.1 -o libpololu.so.1 libpololu.o -lc
		ln -s -f libpololu.so.1 libpololu.so

servo:	servo.o
	$(CC) $(CFLAGS) -o servo servo.o -lpololu

speedo:	speedo.o
	$(CC) $(CFLAGS) -o speedo speedo.o -lpololu -lgps

clean:
	rm -f *.o *.a *~ servo speedo *.so.* *.so
