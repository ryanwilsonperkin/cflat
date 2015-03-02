CC=gcc
LEX=flex
YACC=bison
LDFLAGS=-ll
YFLAGS+=-d

all: cflatc

test: scanner_test

cflatc: scanner.o parser.o ast.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner.o: scanner.c
	$(CC) -o $@ -c $^

scanner.c: scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

parser.o: parser.c
	$(CC) -o $@ -c $^

parser.c: parser.y
	$(YACC) $(YFLAGS) -o $@ $^

ast.o: ast.c
	$(CC) -o $@ -c $^

scanner_test: scanner_test.o scanner.o
	$(CC) $(LDFLAGS) -o $@ $^

scanner_test.o: scanner_test.c
	$(CC) -o $@ -c $^

clean:
	$(RM) *.o scanner.c parser.c
