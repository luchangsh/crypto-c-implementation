hw2: hw2.o stream.o encrypt.o merge.o decrypt.o
	gcc -g hw2.o stream.o encrypt.o merge.o decrypt.o -L/usr/local/lib -lcrypto -lm -o hw2

hw2.o: hw2.c hw2.h
	gcc -g -Wall -c hw2.c

stream.o: stream.c stream.h
	gcc -g -Wall -c stream.c -I/usr/local/include

encrypt.o: encrypt.c encrypt.h
	gcc -g -Wall -c encrypt.c

merge.o: merge.c merge.h
	gcc -g -Wall -c merge.c

decrypt.o: decrypt.c decrypt.h
	gcc -g -Wall -c decrypt.c

clean:
	rm -f *.o hw2
