CFLAGS+=-O2 -Wall -Werror -std=gnu99 -I.

all: hamming black transmit 

hamming black transmit:myerror.o
myerror.o:myerror.h

hamming: hamming.c
	cc -o $@ $(CFLAGS) $(LFLAGS) $?
black:black.c
	cc -o $@ $(CFLAGS) $(LFLAGS) $?
transmit:transmit.c
	cc -o $@ $(CFLAGS) $(LFLAGS) $?

.PHONY:clean
clean:
	rm -f *.o hamming black transmit 
