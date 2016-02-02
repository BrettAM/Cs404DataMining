CC=g++
SDIR=src
ODIR=build
TDIR=test
CFLAGS=-c -Wall -g -O2 -std=gnu++11
LDFLAGS=
MAIN=$(SDIR)/main.cpp
CPPS=$(filter-out $(MAIN), $(wildcard $(SDIR)/*.cpp))
OBJS=$(addprefix $(ODIR)/,$(notdir $(CPPS:.cpp=.o)))
TESTS=$(wildcard $(TDIR)/*.cpp)
EXECUTABLE=nn
INC=-I$(SDIR)
TESTLIB=-I/usr/local/include/UnitTest++ -lUnitTest++
TESTCMD=testrunner

all: test $(EXECUTABLE)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJS)
	$(CC) $(MAIN) $(LDFLAGS) $(OBJS) -o $@

test: $(OBJS) $(TESTS)
	$(CC) $(TESTS) $(INC) $(TESTLIB) $(OBJS) -o $(TESTCMD)
	./$(TESTCMD)

clean:
	rm -f $(OBJS) $(EXECUTABLE) $(TESTCMD)
