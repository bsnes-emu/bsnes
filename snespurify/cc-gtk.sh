g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c phoenix/phoenix.cpp `pkg-config --cflags gtk+-2.0` -DPHOENIX_GTK
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c snespurify.cpp -DPHOENIX_GTK
g++-4.5 -s -o snespurify snespurify.o phoenix.o `pkg-config --libs gtk+-2.0` -lX11
rm *.o
