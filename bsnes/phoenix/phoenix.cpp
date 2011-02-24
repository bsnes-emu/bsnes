#if defined(PHOENIX_WINDOWS)
  #define UNICODE
  #define WINVER 0x0501
  #define _WIN32_WINNT 0x0501
  #define _WIN32_IE 0x0600
  #define NOMINMAX

  #include <windows.h>
  #include <windowsx.h>
  #include <commctrl.h>
  #include <io.h>
  #include <shlobj.h>
#elif defined(PHOENIX_QT)
  #include <QApplication>
  #include <QtGui>
#elif defined(PHOENIX_REFERENCE)
#else
  #error "phoenix: unrecognized target"
#endif

#include "phoenix.hpp"
using namespace nall;

namespace phoenix {
  #include "core/core.cpp"
}
