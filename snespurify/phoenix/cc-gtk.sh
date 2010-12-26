g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c phoenix.cpp `pkg-config --cflags gtk+-2.0` -DPHOENIX_GTK
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c test.cpp -DPHOENIX_GTK
g++-4.5 -s -o test-gtk test.o phoenix.o `pkg-config --libs gtk+-2.0`
rm *.o
