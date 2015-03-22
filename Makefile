CC=gcc
LEX=flex
YACC=bison
LDFLAGS=-ll
YFLAGS+=-d -v

all: cflatc

cflatc: parser.o scanner.o ast.o astprint.o
	$(CC) -o $@ $^ $(LDFLAGS)

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

astprint.o: astprint.c
	$(CC) -o $@ -c $^

clean:
	$(RM) *.o scanner.c parser.c parser.h
