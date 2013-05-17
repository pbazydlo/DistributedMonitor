CXX=mpiCC
OBJS=../../DistributedMonitorBase.o ../../Communication/MpiCommunicationBase.o ../../Communication/ICommunicationBase.o ../../Logging/Logger.o Common.o Master.o

all:	$(OBJS)
	$(CXX) -g $^ -o $@ -lpthread -std=c++11 -Wno-write-strings

$(OBJS): %.o: %.cpp
	$(CXX) -c -g $< -o $@ -std=c++11 -Wno-write-strings

clean:
	rm -f *.o

