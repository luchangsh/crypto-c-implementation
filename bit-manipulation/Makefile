hw1: hw1.o hexdump.o base64.o
	gcc -g hw1.o hexdump.o base64.o -o hw1

hw1.o: hw1.c hw1.h 
	gcc -g -Wall -c hw1.c

hexdump.o: hexdump.c hexdump.h
	gcc -g -Wall -c hexdump.c

base64.o: base64.c base64.h
	gcc -g -Wall -c base64.c

clean:
	rm -f *.o hw1
