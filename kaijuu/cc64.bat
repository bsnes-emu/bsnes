windres kaijuu.rc kaijuu-resource.o
g++ -m64 -I. -std=gnu++0x -O3 -fomit-frame-pointer -c kaijuu.cpp
g++ -m64 -shared -static-libgcc -Wl,kaijuu.def -Wl,-enable-stdcall-fixup -s -o kaijuu64.dll kaijuu.o -lm -lole32 -luuid -lshlwapi
g++ -m64 -I. -std=gnu++0x -O3 -fomit-frame-pointer -o phoenix.o -c phoenix/phoenix.cpp -DPHOENIX_WINDOWS
g++ -m64 -I. -std=gnu++0x -O3 -fomit-frame-pointer -c interface.cpp
g++ -m64 -mwindows -s -o kaijuu64.exe interface.o kaijuu-resource.o phoenix.o -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -lshlwapi
@pause
@del *.o
