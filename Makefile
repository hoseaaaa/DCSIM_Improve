CC = g++
CFLAGS = -std=c++11 -c -Wall -Wextra -g -O0 -D DEBUG
LFLAGS = -std=c++11 -Wall -Wextra -g -O0
IBM_DATA_HOME = /home/yhz/cqpgsim/data_in/ibm_data
THU_DATA_HOME = /home/yhz/cqpgsim/data_in/thu_data
INC = -I./inc
LIB = -L./lib -lcqpg -lcktso # 库的顺序很重要 cqpg 调用 cktso, 所以 cqpg 在前

all: clean libs cqpg_main

libs:
	(cd lib; make)

cqpg_main: cqpg_main.o
	$(CC) $(LFLAGS) -o bin/cqpgsim lib/$< $(INC) $(LIB)

cqpg_main.o: src/cqpg_main.cpp
	$(CC) $(CFLAGS) $< -o lib/$@ $(INC)

clean:
	(cd lib; make clean)
	(cd bin; rm -f *)
	(cd src; rm -f *.mtx)

run:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg1.spice ibmpg1.cqpg ${IBM_DATA_HOME}/ibmpg1.solution

compress:
	make clean; \
	cd ..; \
	tar -zcvf cqpgsim.tar.gz cqpgsim/

run-2:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg2.spice ibmpg2.cqpg ${IBM_DATA_HOME}/ibmpg2.solution

run-3:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg3.spice ibmpg3.cqpg ${IBM_DATA_HOME}/ibmpg3.solution

run-4:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg4.spice ibmpg4.cqpg ${IBM_DATA_HOME}/ibmpg4.solution

run-5:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg5.spice ibmpg5.cqpg ${IBM_DATA_HOME}/ibmpg5.solution

run-6:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg6.spice ibmpg6.cqpg ${IBM_DATA_HOME}/ibmpg6.solution

