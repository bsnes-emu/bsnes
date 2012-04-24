#ifndef PHOENIX_CPP
#define PHOENIX_CPP

#if defined(PHOENIX_WINDOWS)
  #define UNICODE
  #define WINVER 0x0501
  #define _WIN32_WINNT 0x0501
  #define _WIN32_IE 0x0600
  #define __MSVCRT_VERSION__ 0x0601
  #define NOMINMAX

  #include <windows.h>
  #include <windowsx.h>
  #include <commctrl.h>
  #include <io.h>
  #include <shlobj.h>
#elif defined(PHOENIX_QT)
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
#elif defined(PHOENIX_GTK)
  #include <nall/xorg/guard.hpp>
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
  #include <gdk/gdkx.h>
  #include <gdk/gdkkeysyms.h>
  #include <cairo.h>
  #include <X11/Xatom.h>
  #include <nall/xorg/guard.hpp>
#elif defined(PHOENIX_REFERENCE)
#else
  #error "phoenix: unrecognized target"
#endif

#include "phoenix.hpp"
using namespace nall;

namespace phoenix {
  #include "core/core.cpp"
}

#endif
