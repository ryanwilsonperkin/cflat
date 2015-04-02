BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIR = src
OBJS = parser.o scanner.o ast.o astprint.o symbol.o symbolprint.o typecheck.o print.o cflatc.o

CC = gcc
LEX = flex
YACC = bison
LDFLAGS += -ll -largp
CFLAGS += -g -I$(INCLUDE_DIR) -I.
YFLAGS += -d

all: cflatc

cflatc: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^

scanner.c: $(SRC_DIR)/scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

parser.c: $(SRC_DIR)/parser.y
	$(YACC) $(YFLAGS) -o $@ $^

clean:
	$(RM) *.o *.a scanner.c parser.c parser.h
