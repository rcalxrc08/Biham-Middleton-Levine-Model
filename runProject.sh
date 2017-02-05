#!/bin/bash
rm -f *.o
rm -f *.csv
rm -f *.txt
g++ creator.cpp -o problem.o
arr=( $(awk 'BEGIN{srand(); r=rand(); print r, 1-r}') )
touch file.txt
echo $RANDOM >> file.txt
echo $RANDOM >> file.txt
echo "${arr[0]}" >> file.txt
./problem.o<file.txt
echo "$RANDOM" | cat - problem.csv> temp && mv temp problem.csv
# sed -i '/./!d' problem.csv
# sed -i '/./!d' problem.csv
rm -f ConsegnaMPI/problem.csv
rm -f ConsegnaParallelo/problem.csv
rm -f ConsegnaSeriale/problem.csv
cp problem.csv ConsegnaMPI/
cp problem.csv ConsegnaSeriale/
cp problem.csv ConsegnaParallelo/
cd ConsegnaSeriale/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../
cd ConsegnaParallelo/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../
cd ConsegnaMPI/
chmod +x build.sh run.sh
./build.sh
./run.sh
cd ../