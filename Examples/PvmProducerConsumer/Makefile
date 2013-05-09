CXX=g++
PVMINC=$(PVM_ROOT)/include
PVMLIB=$(PVM_ROOT)/lib/$(PVM_ARCH) 
OBJS=../../DistributedMonitorBase.o ../../Communication/PvmCommunicationBase.o ../../Communication/ICommunicationBase.o ../../Logging/Logger.o

all:	$(PVM_HOME)/master $(PVM_HOME)/slave

run:
	$(PVM_HOME)/master

$(PVM_HOME)/master:	Master.cpp Common.cpp $(OBJS)
	$(CXX) -g $^ -o $(PVM_HOME)/master -L$(PVMLIB) -I$(PVMINC) -lpvm3 -lgpvm3 -lpthread -std=c++11 -Wwrite-strings

$(PVM_HOME)/slave:	Slave.cpp Common.cpp $(OBJS)
	$(CXX) -g $^ -o $(PVM_HOME)/slave -L$(PVMLIB) -I$(PVMINC) -lpvm3 -lgpvm3 -lpthread -std=c++11 -Wwrite-strings

$(OBJS): %.o: %.cpp
	$(CXX) -c -g $< -o $@ -Wwrite-strings

clean:
	rm $(PVM_HOME)/master $(PVM_HOME)/slave
	rm -f *.o

