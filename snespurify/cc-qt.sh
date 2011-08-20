moc -i -o phoenix/qt/qt.moc phoenix/qt/qt.moc.hpp
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c phoenix/phoenix.cpp `pkg-config --cflags QtCore QtGui` -DPHOENIX_QT
g++-4.5 -std=gnu++0x -I. -O3 -fomit-frame-pointer -c snespurify.cpp
g++-4.5 -s -o snespurify-qt snespurify.o phoenix.o `pkg-config --libs QtCore QtGui`
rm *.o
