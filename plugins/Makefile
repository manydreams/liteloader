test: %.o test.so Makefile
	gcc $*.o -O3 -o test && rm $*.o

%.o: Makefile
	gcc -c -O3 $*.c

test.so: test.c Makefile
	gcc -O3 -fPIC -shared plug_test.c -o plug.so
