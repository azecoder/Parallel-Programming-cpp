echo ""
echo "Remove Results Folder"
echo ""
make clean
echo ""
echo "Create Results Folder"
echo ""
mkdir results
echo ""
echo "Build Cpp Codes"
echo ""
make all
echo ""


echo ""
echo ""
echo "Start: SEQUENTIAL"
echo ""
echo "./seq.o 100000 > results/seq.txt"
./seq.o 100000 > results/seq.txt
echo ""
echo "Finished: SEQUENTIAL"


echo ""
echo ""
echo "Start: PARALLEL"
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par.o 100000 $nw
done > results/par.txt
echo ""
echo "Finished: PARALLEL"


echo ""
echo ""
echo "Start: FF PARALLEL FOR"
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par_fastflow.o 100000 $nw
done > results/par_fastflow.txt
echo ""
echo "Finished: FF PARALLEL FOR"


echo ""
echo ""
echo "Start: OMP"
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par_openmp.o 100000 $nw
done > results/par_openmp.txt
echo ""
echo "Finished: OMP"
echo ""
echo ""