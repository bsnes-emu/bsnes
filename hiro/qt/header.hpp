#include <QApplication>
#include <QtGui>
#include <QtWidgets>
#undef foreach

#if defined(DISPLAY_XORG)
#include <nall/xorg/guard.hpp>
#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#undef XK_MISCELLANY
#undef XK_LATIN1
#include <nall/xorg/guard.hpp>
#endif
