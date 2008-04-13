g++ -c ssmpgen.cpp -I../../../lib
g++ -c ../../../lib/nall/string.cpp -I../../../lib
g++ -o ssmpgen ssmpgen.o string.o
rm *.o
