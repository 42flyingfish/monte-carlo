CXX:=g++
LDLIBS := -lpthread

all: monte

monte: monte.o
	$(CXX) -o $@ $^  $(LDLIBS)

monte.o: monte.cpp

.PHONY: all
