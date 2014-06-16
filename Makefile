# Makefile
# Evan Sebastian <evanlhoini@gmail.com>

CC = c++
CFLAGS = -g -Wall -std=c++11
LDFLAGS =
INCLUDES = -I.
SOURCE = $(shell find *.cpp)
OBJ = $(SOURCE:.cpp=.o)
RM = rm -f

###

all: stream

stream: $(OBJ)
	$(CC) $(LDFLAGS) -o stream $(OBJ) && ./stream && make clean

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)

.PHONY : clean
