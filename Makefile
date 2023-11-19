test: *.o test.so Makefile
	gcc *.o -o test && rm *.o

*.o: *.c Makefile
	gcc -c *.c

test.so: test.c Makefile
	gcc -fPIC -shared plug_test.c -o plug.so
