BIN_DIR = bin
INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR=$(SRC_DIR)/test
GTEST_DIR=vendor/gtest-1.7.0
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CC = gcc
LEX = flex
YACC = bison
LDFLAGS += -ll -lm
CFLAGS += -g -I$(INCLUDE_DIR) -I.
CPPFLAGS += -isystem $(GTEST_DIR)/include -I$(INCLUDE_DIR) -I.
CXXFLAGS += -g -Wall -pthread
YFLAGS += -d

ifeq ($(shell uname), Darwin)
	LDFLAGS += -largp
endif

OBJS = parser.o scanner.o ast.o astprint.o symbol.o symbolprint.o typecheck.o print.o cflatc.o
TEST_OBJS = symbol_test.o symbol.o ast.o typecheck.o

all: $(BIN_DIR)/cflatc

test: $(BIN_DIR)/test_cflatc

$(BIN_DIR)/cflatc: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^

scanner.c: $(SRC_DIR)/scanner.l
	$(LEX) $(LFLAGS) -o $@ $^ 

parser.c: $(SRC_DIR)/parser.y
	$(YACC) $(YFLAGS) -o $@ $^

$(BIN_DIR)/test_cflatc: $(TEST_OBJS) gtest_main.a
	mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

symbol_test.o: $(TEST_DIR)/symbol_test.cc $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(TEST_DIR)/symbol_test.cc

# Targets for building Google Test Framework
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) *.o *.a scanner.c parser.c parser.h $(BIN_DIR)/*
