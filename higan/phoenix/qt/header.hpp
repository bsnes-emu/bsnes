#include <QApplication>
#include <QtGui>
#include <nall/xorg/guard.hpp>
#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#undef XK_MISCELLANY
#undef XK_LATIN1
#include <nall/xorg/guard.hpp>

//Qt 4.8.0 and earlier improperly define the QLOCATION macro
//in C++11, it is detected as a malformed user-defined literal
//below is a workaround to fix compilation errors caused by this
#undef  QLOCATION
#define QLOCATION "\0" __FILE__ ":" QTOSTRING(__LINE__)
