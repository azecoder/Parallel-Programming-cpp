CXX 	= g++ 
FLAGS 	= -O3 -std=c++17 -pthread
OMPFLAG	= -fopenmp
FF		= -finline-functions -DNDEBUG -Wall -I /home/azecoder/fastflow/
# FF		= -finline-functions -DNDEBUG

TARGETS = seq par par_fastflow par_openmp

all:
	$(TARGETS)
seq:
	$(CXX) $(FLAGS) seq.cpp -o seq.o
par:
	$(CXX) $(FLAGS) par.cpp -o par.o
par_fastflow:
	$(CXX) $(FLAGS) par_fastflow.cpp -o par_fastflow.o $(FF)
par_openmp:
	$(CXX) $(FLAGS) par_openmp.cpp -o par_openmp.o $(OMPFLAG)
