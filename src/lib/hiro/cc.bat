mingw32-g++ -c test/test.cpp -I. -I../
mingw32-g++ -c hiro.cpp -I. -I../
mingw32-g++ -c ../nall/string.cpp -I. -I../
mingw32-g++ test.o hiro.o string.o -o test_app.exe -lkernel32 -luser32 -lgdi32 -ladvapi32 -lcomctl32 -lcomdlg32
@pause
@del *.o
