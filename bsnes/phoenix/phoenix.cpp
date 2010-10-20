#if defined(PHOENIX_WINDOWS)
  #define UNICODE
  #define WINVER 0x0501
  #define _WIN32_WINNT 0x0501
  #define _WIN32_IE 0x0600
  #define NOMINMAX
#endif

#include "phoenix.hpp"

#if defined(PHOENIX_WINDOWS)
  #include "windows/windows.cpp"
#elif defined(PHOENIX_GTK)
  #include "gtk/gtk.cpp"
#elif defined(PHOENIX_QT)
  #include "qt/qt.cpp"
#endif
