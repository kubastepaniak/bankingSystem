all: banking

banking: banking.o inputreads.o dboperations.o
	gcc -g $^ -o $@ -lm

.c.o: 
	gcc -g -Wall -pedantic -std=c99 -c -D_REENTRANT $<

inputreads.o: inputreads.c inputreads.h accstruct.h dboperations.h

dboperations.o: dboperations.c dboperations.h accstruct.h

banking.o: banking.c inputreads.h dboperations.h accstruct.h

clean:
	-rm banking.o inputreads.o dboperations.o banking