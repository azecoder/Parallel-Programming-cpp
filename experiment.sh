mkdir results

make all

echo ""
echo ""
echo "Start: SEQUENTIAL"
echo ""
./seq.o 100000 > results/seq.txt
echo ""
echo ""
echo "Finished: SEQUENTIAL"


echo ""
echo ""
echo "Start: PARALLEL"
echo ""
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par.o 100000 $nw > results/par.txt
done
echo ""
echo ""
echo "Finished: PARALLEL"


echo ""
echo ""
echo "Start: FF PARALLEL FOR"
echo ""
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par_fastflow.o 100000 $nw > results/par_fastflow.txt
done
echo ""
echo ""
echo "Finished: FF PARALLEL FOR"


echo ""
echo ""
echo "Start: OMP"
echo ""
echo ""
for nw in 1 2 4 8 16 32 64
    do
    ./par_openmp.o 100000 $nw > results/par_openmp.txt
done
echo ""
echo ""
echo "Finished: OMP"
