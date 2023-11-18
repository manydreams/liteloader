test: *.o Makefile
	gcc *.o -o test && rm *.o
*.o: *.c Makefile
	gcc -c *.c
