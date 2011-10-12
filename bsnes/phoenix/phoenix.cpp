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
#elif defined(PHOENIX_GTK)
  #define None
  #define Window X11Window
  #define X11None 0L

  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
  #include <gdk/gdkx.h>
  #include <cairo.h>
  #include <gdk/gdkkeysyms.h>
  #include <X11/Xatom.h>

  #undef None
  #undef Window
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
