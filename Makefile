CC = g++
CFLAGS = -std=c++11 -c -Wall -Wextra -g -O0 -D DEBUG
LFLAGS = -std=c++11 -Wall -Wextra -g -O0
DCSIM := $(shell echo $$DCSIM_HOME)
IBM_DATA_HOME := $(DCSIM)/data_in/ibm_data
THU_DATA_HOME := $(DCSIM)/data_in/thu_data
SEU_DATA_HOME := $(DCSIM)/data_in/seu_data

IBM_DATA_OUT  := $(DCSIM)/data_out/ibm_data_cq
THU_DATA_OUT  := $(DCSIM)/data_out/thu_data_cq
SEU_DATA_OUT  := $(DCSIM)/data_out/seu_data_cq

INC = -I./inc
LIB = -L./lib -lcqpg -lcktso # 库的顺序很重要 cqpg 调用 cktso, 所以 cqpg 在前


IBM-TARGETS := ibmpg1 ibmpg2 ibmpg3 ibmpg4 ibmpg5 ibmpg6 
SEU-TARGETS := seupg1 seupg2 seupg3 seupg4 seupg5 seupg6 seupg7 seupg8 seupg9 seupg10
THU-TARGETS := thupg1 thupg2 thupg3 thupg4 thupg5 thupg6 thupg7 thupg8 thupg9 thupg10

.PHONY: thu-run ibm-run seu-run $(THU-TARGETS) $(IBM-TARGETS) $(SEU-TARGETS) 

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
	@for i in $$(seq 1 6); do \
        echo "Running ibmpg$$i..."; \
        make ibmpg$$i || true; \
    done

thu-run:
	@for i in $$(seq 1 10); do \
        echo "Running thupg$$i..."; \
        make thupg$$i || true; \
    done

# seu-run:
# 	@for i in $$(seq 1 6); do \
#         echo "Running thu-$$i..."; \
#         make thu-$$i || true; \
#     done

$(IBM-TARGETS):
	cd bin && \
	./cqpgsim $(IBM_DATA_HOME)/$@.spice $(IBM_DATA_OUT)/$@.cqpg $(IBM_DATA_HOME)/$@.solution $(IBM_DATA_OUT)/$@/thupg.G.txt $(IBM_DATA_OUT)/$@/thupg.u.txt $(IBM_DATA_OUT)/$@/thupg.x.txt > $(IBM_DATA_OUT)/$@/$@.info

# thu's solution 没有用处，此处仅是方便输入，以免在输入格式判定时失败
$(THU-TARGETS):
	cd bin && \
	./cqpgsim $(THU_DATA_HOME)/$@.spice $(THU_DATA_OUT)/$@.cqpg $(IBM_DATA_HOME)/ibmpg1.solution $(THU_DATA_OUT)/$@/thupg.G.txt $(THU_DATA_OUT)/$@/thupg.u.txt $(THU_DATA_OUT)/$@/thupg.x.txt > $(THU_DATA_OUT)/$@/$@.info

##solution 没有用处，此处仅是方便输入，以免在输入格式判定时失败
# $(THU-TARGETS):
# 	cd bin && \
# 	./cqpgsim $(THU_DATA_HOME)/$@.spice $(THU_DATA_OUT)/$@.cqpg $(IBM_DATA_HOME)/ibmpg1.solution $(THU_DATA_OUT)/$@/thupg.G.txt $(THU_DATA_OUT)/$@/thupg.u.txt $(THU_DATA_OUT)/$@/thupg.x.txt > $(THU_DATA_OUT)/$@/$@.info
