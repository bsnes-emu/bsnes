moc -i -o qt/qt.moc qt/qt.moc.hpp
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c phoenix.cpp `pkg-config --cflags QtCore QtGui` -DPHOENIX_QT
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c test.cpp -DPHOENIX_QT
g++-4.5 -s -o test-qt test.o phoenix.o `pkg-config --libs QtCore QtGui`
rm *.o
#rm qt/qt.moc
