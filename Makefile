CC=gcc
LEX=flex
LDFLAGS=-ll

all: cflatc

test: scanner_test

cflatc: scanner.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner.o: scanner.c
	$(CC) -o $@ -c $^

scanner.c: scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

scanner_test: scanner_test.o scanner.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner_test.o: scanner_test.c
	$(CC) -o $@ -c $^

clean:
	$(RM) *.o scanner.c
