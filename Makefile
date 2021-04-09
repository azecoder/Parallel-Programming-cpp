CXX 	= g++ 
FLAGS 	= -O3 -std=c++17 -pthread
OMPFLAG	= -fopenmp
FF		= -Wall -DNO_DEFAULT_MAPPING
# FF		= -I $(HOME)/fastflow/ -finline-functions

seq:
	$(CXX) $(FLAGS) seq.cpp -o seq.o
par:
	$(CXX) $(FLAGS) par.cpp -o par.o
mw:
	$(CXX) $(FLAGS) master_worker.cpp -o master_worker.o
ewc:
	$(CXX) $(FLAGS) emitter_collector.cpp -o emitter_collector.o
par_fastflow:
	$(CXX) $(FLAGS) par_fastflow.cpp -o par_fastflow.o $(FF)
par_openmp:
	$(CXX) $(FLAGS) par_openmp.cpp -o par_openmp.o $(OMPFLAG)


TARGETS = seq par par_fastflow par_openmp

all:
	make $(TARGETS)

clean: 
	$(RM) *.o *~ -r results