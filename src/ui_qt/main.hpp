#define UNICODE
#define QT_NO_DEBUG
#define QT_CORE_LIB
#define QT_GUI_LIB
#define QT_THREAD_SUPPORT

#include <QApplication>
#include <QtGui>
//Q_IMPORT_PLUGIN(QJpegPlugin)
//Q_IMPORT_PLUGIN(QMngPlugin)

#include <../base.hpp>

#include <nall/config.hpp>
#include <nall/input.hpp>
#include <nall/ups.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;

#include <libreader/libreader.hpp>
#include <libreader/filereader.hpp>

#if defined(GZIP_SUPPORT)
  #include <libreader/gzreader.hpp>
  #include <libreader/zipreader.hpp>
#endif

#if defined(JMA_SUPPORT)
  #include <libreader/jmareader.hpp>
#endif

#include <libfilter/libfilter.hpp>

#include "config.hpp"
#include "interface.hpp"
#include "application/application.moc"
#include "input/input.hpp"
#include "utility/utility.hpp"

struct Style {
  enum {
    WindowMargin     = 5,
    WidgetSpacing    = 5,
    SeparatorSpacing = 5,
  };
};
