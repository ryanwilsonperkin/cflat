CC=gcc
LEX=flex
YACC=bison
LDFLAGS=-ll -largp
CFLAGS=-g
YFLAGS+=-d -v

all: cflatc

cflatc: parser.o scanner.o ast.o astprint.o symbol.o cflatc.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

cflatc.o: cflatc.c
	$(CC) $(CFLAGS) -o $@ -c $^

scanner.o: scanner.c
	$(CC) $(CFLAGS) -o $@ -c $^

scanner.c: scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

parser.o: parser.c
	$(CC) $(CFLAGS) -o $@ -c $^

parser.c: parser.y
	$(YACC) $(YFLAGS) -o $@ $^

ast.o: ast.c
	$(CC) $(CFLAGS) -o $@ -c $^

astprint.o: astprint.c
	$(CC) $(CFLAGS) -o $@ -c $^

symbol.o: symbol.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) *.o scanner.c parser.c parser.h
