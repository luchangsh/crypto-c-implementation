hw3: hw3.o keygen.o stream.o crypt.o invkey.o histo.o solve.o
	gcc -g hw3.o keygen.o stream.o crypt.o invkey.o histo.o solve.o -L/usr/local/lib -lcrypto -lm -o hw3

hw3.o: hw3.c hw3.h
	gcc -g -Wall -c hw3.c

keygen.o: keygen.c keygen.h
	gcc -g -Wall -c keygen.c

stream.o: stream.c stream.h
	gcc -g -Wall -c stream.c -I/usr/local/include

crypt.o: crypt.c crypt.h
	gcc -g -Wall -c crypt.c

invkey.o: invkey.c invkey.h
	gcc -g -Wall -c invkey.c

histo.o: histo.c histo.h
	gcc -g -Wall -c histo.c

solve.o: solve.c solve.h
	gcc -g -Wall -c solve.c

clean:
	rm -f *.o hw3
