CC = g++
CFLAGS = -std=c++11 -c -Wall -Wextra -fPIC -g -O0
LFLAGS = -std=c++11 -Wall -Wextra -shared -g -O0

INC = -I../inc
LIB = -lcktso

all: libcqpg.so

libcqpg.so: cqpg_parse.o cqpg_dc.o cqpg_tran.o cqpg_print.o
	$(CC) $(LFLAGS) $^ -o $@ 

cqpg_parse.o: ../src/cqpg_parse.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC)
cqpg_dc.o: ../src/cqpg_dc.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC) $(LIB)
cqpg_tran.o: ../src/cqpg_tran.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC) $(LIB)
cqpg_print.o: ../src/cqpg_print.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INC)

clean:
	rm -f *.o libcqpg.so
