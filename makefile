CC=g++
SDIR=src
ODIR=build
CFLAGS=-c -Wall -g -O2 -std=gnu++11
LDFLAGS=
CPPS=$(wildcard $(SDIR)/*.cpp)
OBJS=$(addprefix $(ODIR)/,$(notdir $(CPPS:.cpp=.o)))
EXECUTABLE=nn
INC=-I$(SDIR)

all: $(OBJS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
