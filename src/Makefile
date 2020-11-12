all: genprimes

genprimes: genprimes.c
	mpicc -g -Wall -std=c99 genprimes.c -o genprimes -lm

clean:
	rm genprimes *.txt
