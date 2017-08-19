hw4: hw4.o tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o
	gcc -g hw4.o tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o -o hw4

hw4.o: hw4.c hw4.h
	gcc -g -Wall -c hw4.c

tablecheck.o: tablecheck.c tablecheck.h
	gcc -g -Wall -c tablecheck.c

encrypt.o: encrypt.c encrypt.h
	gcc -g -Wall -c encrypt.c

decrypt.o: decrypt.c decrypt.h
	gcc -g -Wall -c decrypt.c

encrypt3.o: encrypt3.c encrypt3.h
	gcc -g -Wall -c encrypt3.c

decrypt3.o: decrypt3.c decrypt3.h
	gcc -g -Wall -c decrypt3.c

clean:
	rm -f *.o hw4
