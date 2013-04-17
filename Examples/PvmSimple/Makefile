PVMINC=$(PVM_ROOT)/include
PVMLIB=$(PVM_ROOT)/lib/$(PVM_ARCH) 

all:	$(PVM_HOME)/master $(PVM_HOME)/slave

run:
	$(PVM_HOME)/master

$(PVM_HOME)/master:	Master.cpp Common.h
	$(CC) -g Master.cpp -o $(PVM_HOME)/master -L$(PVMLIB) -I$(PVMINC) -lpvm3 -lgpvm3

$(PVM_HOME)/slave:	Slave.cpp Common.h
	$(CC) -g Slave.cpp -o $(PVM_HOME)/slave -L$(PVMLIB) -I$(PVMINC) -lpvm3 -lgpvm3

clean:
	rm $(PVM_HOME)/master $(PVM_HOME)/slave
	rm -f *.o

