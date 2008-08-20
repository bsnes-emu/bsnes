clear
g++ -c test/test.cpp -I. -I../
g++ -c hiro.cpp `pkg-config --cflags gtk+-2.0` -I. -I../
g++ -c ../nall/string.cpp -I. -I../
g++ test.o hiro.o string.o -o test_app `pkg-config --libs gtk+-2.0` -lXtst
rm *.o
