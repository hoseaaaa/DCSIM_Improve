CC = g++
CFLAGS = -std=c++11 -c -Wall -Wextra -g -O0 -D DEBUG
LFLAGS = -std=c++11 -Wall -Wextra -g -O0
DCSIM := $(shell echo $$DCSIM_HOME)
IBM_DATA_HOME := $(DCSIM)/data_in/ibm_data
THU_DATA_HOME := $(DCSIM)/data_in/thu_data
IBM_DATA_OUT  := $(DCSIM)/data_out/ibm_data_cq
THU_DATA_OUT  := $(DCSIM)/data_out/thu_data_cq
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

ibm-run:
	# @for i in {1..6}; do \
		# echo "Running ibm-$$i..."; \
		# make ibm-$$i || true; \
	# done
	make ibm-1 || true;
	make ibm-2 || true;
	make ibm-3 || true;
	make ibm-4 || true;
	make ibm-5 || true;
	make ibm-6 || true;
thu-run:
	make thu-1  || true;
	make thu-2  || true;
	make thu-3  || true;
	make thu-4  || true;
	make thu-5  || true;
	make thu-6  || true;
	make thu-7  || true;
	make thu-8  || true;
	make thu-9  || true;
	make thu-10 || true;

compress:
	make clean; \
	cd ..; \
	tar -zcvf cqpgsim.tar.gz cqpgsim/

ibm-1:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg1.spice ${IBM_DATA_OUT}/ibmpg1.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${IBM_DATA_OUT}/ibmpg1/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg1/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg1/ibmpg.x.txt > ${IBM_DATA_OUT}/ibmpg1/ibmpg1.info

ibm-2:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg2.spice ${IBM_DATA_OUT}/ibmpg2.cqpg ${IBM_DATA_HOME}/ibmpg2.solution ${IBM_DATA_OUT}/ibmpg2/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg2/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg2/ibmpg.x.txt  > ${IBM_DATA_OUT}/ibmpg2/ibmpg2.info

ibm-3:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg3.spice ${IBM_DATA_OUT}/ibmpg3.cqpg ${IBM_DATA_HOME}/ibmpg3.solution ${IBM_DATA_OUT}/ibmpg3/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg3/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg3/ibmpg.x.txt > ${IBM_DATA_OUT}/ibmpg3/ibmpg3.info

ibm-4:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg4.spice ${IBM_DATA_OUT}/ibmpg4.cqpg ${IBM_DATA_HOME}/ibmpg4.solution ${IBM_DATA_OUT}/ibmpg4/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg4/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg4/ibmpg.x.txt  > ${IBM_DATA_OUT}/ibmpg4/ibmpg4.info

ibm-5:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg5.spice ${IBM_DATA_OUT}/ibmpg5.cqpg ${IBM_DATA_HOME}/ibmpg5.solution ${IBM_DATA_OUT}/ibmpg5/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg5/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg5/ibmpg.x.txt > ${IBM_DATA_OUT}/ibmpg5/ibmpg5.info

ibm-6:
	cd bin; \
	./cqpgsim ${IBM_DATA_HOME}/ibmpg6.spice ${IBM_DATA_OUT}/ibmpg6.cqpg ${IBM_DATA_HOME}/ibmpg6.solution ${IBM_DATA_OUT}/ibmpg6/ibmpg.G.txt ${IBM_DATA_OUT}/ibmpg6/ibmpg.u.txt ${IBM_DATA_OUT}/ibmpg6/ibmpg.x.txt  > ${IBM_DATA_OUT}/ibmpg6/ibmpg6.info

# thu's solution 没有用处，此处仅是方便输入，以免在输入格式判定时失败

thu-1:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg1.spice ${THU_DATA_OUT}/thupg1.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg1/thupg.G.txt ${THU_DATA_OUT}/thupg1/thupg.u.txt ${THU_DATA_OUT}/thupg1/thupg.x.txt > ${THU_DATA_OUT}/thupg1/thupg1.info
thu-2:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg2.spice ${THU_DATA_OUT}/thupg2.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg2/thupg.G.txt ${THU_DATA_OUT}/thupg2/thupg.u.txt ${THU_DATA_OUT}/thupg2/thupg.x.txt > ${THU_DATA_OUT}/thupg2/thupg2.info
thu-3:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg3.spice ${THU_DATA_OUT}/thupg3.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg3/thupg.G.txt ${THU_DATA_OUT}/thupg3/thupg.u.txt ${THU_DATA_OUT}/thupg3/thupg.x.txt > ${THU_DATA_OUT}/thupg3/thupg3.info
thu-4:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg4.spice ${THU_DATA_OUT}/thupg4.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg4/thupg.G.txt ${THU_DATA_OUT}/thupg4/thupg.u.txt ${THU_DATA_OUT}/thupg4/thupg.x.txt > ${THU_DATA_OUT}/thupg4/thupg4.info
thu-5:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg5.spice ${THU_DATA_OUT}/thupg5.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg5/thupg.G.txt ${THU_DATA_OUT}/thupg5/thupg.u.txt ${THU_DATA_OUT}/thupg5/thupg.x.txt > ${THU_DATA_OUT}/thupg5/thupg5.info
thu-6:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg6.spice ${THU_DATA_OUT}/thupg6.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg6/thupg.G.txt ${THU_DATA_OUT}/thupg6/thupg.u.txt ${THU_DATA_OUT}/thupg6/thupg.x.txt > ${THU_DATA_OUT}/thupg6/thupg6.info
thu-7:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg7.spice ${THU_DATA_OUT}/thupg7.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg7/thupg.G.txt ${THU_DATA_OUT}/thupg7/thupg.u.txt ${THU_DATA_OUT}/thupg7/thupg.x.txt > ${THU_DATA_OUT}/thupg7/thupg7.info
thu-8:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg8.spice ${THU_DATA_OUT}/thupg8.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg8/thupg.G.txt ${THU_DATA_OUT}/thupg8/thupg.u.txt ${THU_DATA_OUT}/thupg8/thupg.x.txt > ${THU_DATA_OUT}/thupg8/thupg8.info
thu-9:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg9.spice ${THU_DATA_OUT}/thupg9.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg9/thupg.G.txt ${THU_DATA_OUT}/thupg9/thupg.u.txt ${THU_DATA_OUT}/thupg9/thupg.x.txt > ${THU_DATA_OUT}/thupg9/thupg9.info
thu-10:
	cd bin; \
	./cqpgsim ${THU_DATA_HOME}/thupg10.spice ${THU_DATA_OUT}/thupg10.cqpg ${IBM_DATA_HOME}/ibmpg1.solution ${THU_DATA_OUT}/thupg10/thupg.G.txt ${THU_DATA_OUT}/thupg10/thupg.u.txt ${THU_DATA_OUT}/thupg1/thupg.x.txt > ${THU_DATA_OUT}/thupg10/thupg10.info
