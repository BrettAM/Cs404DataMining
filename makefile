CC=g++
SDIR=src
ODIR=build
TDIR=test
CFLAGS=-Wall -g -O2 -std=gnu++11
LDFLAGS=
MAIN=$(SDIR)/main.cpp
HPPS=$(wildcard $(SDIR)/*.hpp)
CPPS=$(filter-out $(MAIN), $(wildcard $(SDIR)/*.cpp))
OBJS=$(addprefix $(ODIR)/,$(notdir $(CPPS:.cpp=.o)))
TESTS=$(wildcard $(TDIR)/*.cpp)
EXECUTABLE=nn
INC=-I$(SDIR)
TESTLIB=-I/usr/local/include/UnitTest++ -lUnitTest++
TESTCMD=testrunner
OUTPUTTAR=BrettAM_NN.tar

all: init $(EXECUTABLE)

init:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(MAIN) $(HPPS) $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(MAIN) $(OBJS) -o $@

test: $(OBJS) $(TESTS)
	$(CC) $(TESTS) $(INC) $(TESTLIB) $(OBJS) -o $(TESTCMD)
	./$(TESTCMD)

tar: test
	tar -cf $(OUTPUTTAR) ./makefile ./src

clean:
	rm -f $(OUTPUTTAR) $(OBJS) $(EXECUTABLE) $(TESTCMD)
