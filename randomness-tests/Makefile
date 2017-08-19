hw7: hw7.o rc4.o x1.o x2.o x3.o x4.o x5.o
	gcc -g hw7.o rc4.o x1.o x2.o x3.o x4.o x5.o -L/usr/local/lib -lcrypto -lm -o hw7

hw7.o: hw7.c hw7.h
	gcc -g -Wall -c hw7.c

rc4.o: rc4.c rc4.h
	gcc -g -Wall -c rc4.c -I/usr/local/include

x1.o: x1.c x1.h
	gcc -g -Wall -c x1.c

x2.o: x2.c x2.h
	gcc -g -Wall -c x2.c

x3.o: x3.c x3.h
	gcc -g -Wall -c x3.c

x4.o: x4.c x4.h
	gcc -g -Wall -c x4.c

x5.o: x5.c x5.h
	gcc -g -Wall -c x5.c

clean:
	rm -f *.o hw7
