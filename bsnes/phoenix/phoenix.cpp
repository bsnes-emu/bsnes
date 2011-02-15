#include "phoenix.hpp"
using namespace nall;

#if defined(PHOENIX_QT)
  #include <QApplication>
  #include <QtGui>
#endif

namespace phoenix {
  #include "core/core.cpp"
}
