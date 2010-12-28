#include <nall/array.hpp>
#include <nall/foreach.hpp>
#include <nall/function.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/vector.hpp>

#if defined(PHOENIX_WINDOWS)
  #include "windows/windows.hpp"
#elif defined(PHOENIX_GTK)
  #include "gtk/gtk.hpp"
#elif defined(PHOENIX_QT)
  #include "qt/qt.hpp"
#endif
