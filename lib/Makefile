CC = g++
CFLAGS = -std=c++11 -c -Wall -Wextra -fPIC -g -O0
LFLAGS = -std=c++11 -Wall -Wextra -shared -g -O0

INC = -I../inc
LIB = -lcktso

all: libcqpg.so

libcqpg.so: cqpg_parse.o  y_para_output.o
	$(CC) $(LFLAGS) $^ -o $@ 

cqpg_parse.o: ../src/cqpg_parse.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC)
y_para_output.o: ../src/y_para_output.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC)

clean:
	rm -f *.o libcqpg.so
