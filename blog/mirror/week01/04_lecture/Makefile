CC	 = 3c

.PHONY: all
all: derpcalc

derpcalc:	derpcalc.o stack.o
derpcalc.o:	derpcalc.c stack.h
stack.o:	stack.c stack.h

.PHONY: clean
clean:
	-rm -f derpcalc derpcalc.o stack.o
