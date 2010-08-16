@windres resource.rc resource.o
@mingw32-g++ -std=gnu++0x -mwindows -s -O3 -fomit-frame-pointer -I.. -o ../out/bsnes launcher.cpp resource.o
@del *.o
@pause
