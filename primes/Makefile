hw6: hw6.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o
	gcc -g hw6.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o -L/usr/local/lib -lcrypto -lm -o hw6
lm -o hw6

hw6.o: hw6.c hw6.h
	gcc -g -Wall -c hw6.c

primes.o: primes.c primes.h
	gcc -g -Wall -c primes.c

trialdiv.o: trialdiv.c trialdiv.h
	gcc -g -Wall -c trialdiv.c -I/usr/local/include

millerrabin.o: millerrabin.c millerrabin.h
	gcc -g -Wall -c millerrabin.c -I/usr/local/include

rndsearch.o: rndsearch.c rndsearch.h
	gcc -g -Wall -c rndsearch.c -I/usr/local/include

maurer.o: maurer.c maurer.h
	gcc -g -Wall -c maurer.c -I/usr/local/include

clean:
	rm -f *.o hw6
