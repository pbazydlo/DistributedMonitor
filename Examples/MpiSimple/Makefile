CXX=mpiCC
OBJS=../../DistributedMonitorBase.o ../../Communication/MpiCommunicationBase.o ../../Communication/ICommunicationBase.o ../../Logging/Logger.o Master.o

all:	Common.h $(OBJS)
	$(CXX) -g $^ -o $@ -lpthread -std=c++11 -Wno-write-strings

$(OBJS): %.o: %.cpp
	$(CXX) -c -g $< -o $@ -Wno-write-strings

clean:
	rm -f *.o

