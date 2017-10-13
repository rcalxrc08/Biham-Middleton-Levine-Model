#!/bin/bash
rm -f *.o
rm -f *.csv
rm -f *.txt
g++-6 creator.cpp -o problem.o
arr=( $(awk 'BEGIN{srand(); r=rand(); print r, 1-r}') )
touch file.txt
echo $RANDOM >> file.txt
echo 100 >> file.txt
echo "${arr[0]}" >> file.txt
./problem.o<file.txt
echo "$RANDOM" | cat - problem.csv> temp && mv temp problem.csv
echo >> problem.csv
 sed -i '/./!d' problem.csv
 sed -i '/./!d' problem.csv
rm -f MPI/problem.csv
rm -f OpenMP/problem.csv
rm -f Serial/problem.csv
cp problem.csv MPI/
cp problem.csv Serial/
cp problem.csv OpenMP/
cd Serial/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../
cd OpenMP/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../
cd MPI/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../