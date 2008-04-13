g++ -c scpugen.cpp -I../../../lib
g++ -c ../../../lib/nall/string.cpp -I../../../lib
g++ -o scpugen scpugen.o string.o
rm *.o
