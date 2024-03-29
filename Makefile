CXX=g++
XLOGINXX=xzhuko01

SRC=**.cpp
BIN=kry

CXXFLAGS:=-Wall -Wextra -Wsuggest-override -Wnull-dereference -Wshadow -Wold-style-cast -pedantic -lgmpxx -lgmp -std=c++20


LINK.o = $(LINK.cpp)

all: CXXFLAGS += -Ofast -march=native -flto
all: kry

debug: CXXFLAGS += -g3 -fsanitize=address,undefined -fno-omit-frame-pointer
debug: kry

kry: $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o $(BIN)

pack:
	zip $(XLOGINXX).zip *.cpp *.hpp Makefile doc.pdf

dep:
	g++ *.cpp -MM >> Makefile

clean:
	rm kry