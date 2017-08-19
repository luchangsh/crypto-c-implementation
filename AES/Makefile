hw5: hw5.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o
	gcc -g hw5.o tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o -o hw5

hw5.o: hw5.c hw5.h
	gcc -g -Wall -c hw5.c

tablecheck.o: tablecheck.c tablecheck.h
	gcc -g -Wall -c tablecheck.c

modprod.o: modprod.c modprod.h
	gcc -g -Wall -c modprod.c

keyexpand.o: keyexpand.c keyexpand.h
	gcc -g -Wall -c keyexpand.c

encrypt.o: encrypt.c encrypt.h
	gcc -g -Wall -c encrypt.c

decrypt.o: decrypt.c decrypt.h
	gcc -g -Wall -c decrypt.c

inverse.o: inverse.c inverse.h
	gcc -g -Wall -c inverse.c

clean:
	rm -f *.o hw5
