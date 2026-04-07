CC = cc
CFLAGS = -O2 -Wall -fPIC -I./ -L./

all: libpololu.a libpololu.so servo

libpololu.a: libpololu.o
	ar rcs libpololu.a libpololu.o

libpololu.so: libpololu.o
	$(CC) -shared -fPIC -Wl,-soname,libpololu.so.1 -o libpololu.so.1 libpololu.o
	ln -sf libpololu.so.1 libpololu.so

servo: servo.o libpololu.a
	$(CC) $(CFLAGS) -o servo servo.o libpololu.a

clean:
	rm -f *.o *.a *.so *.so.* servo

.PHONY: all clean
