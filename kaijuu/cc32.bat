windres --target=pe-i386 kaijuu.rc kaijuu-resource.o
g++ -m32 -I. -std=gnu++0x -O3 -fomit-frame-pointer -c kaijuu.cpp
g++ -m32 -shared -static-libgcc -Wl,kaijuu.def -Wl,-enable-stdcall-fixup -s -o kaijuu32.dll kaijuu.o -lm -lole32 -luuid -lshlwapi
g++ -m32 -I. -std=gnu++0x -O3 -fomit-frame-pointer -o phoenix.o -c phoenix/phoenix.cpp -DPHOENIX_WINDOWS
g++ -m32 -I. -std=gnu++0x -O3 -fomit-frame-pointer -c interface.cpp
g++ -m32 -mwindows -s -o kaijuu32.exe interface.o kaijuu-resource.o phoenix.o -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -lshlwapi
@pause
@del *.o
