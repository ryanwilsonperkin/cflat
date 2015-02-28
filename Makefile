CC=gcc
LEX=flex
LDFLAGS=-ll

all: cflatc

cflatc: scanner.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner.o: scanner.c
	$(CC) -o $@ -c $^

scanner.c: scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

clean:
	$(RM) *.o scanner.c
