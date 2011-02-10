#include <QApplication>
#include <QtGui>

#include <nall/config.hpp>
#include <nall/platform.hpp>
using namespace nall;

namespace phoenix {

#include "qt.moc.hpp"
#include "qt.moc"

#include "settings.cpp"
#include "object.cpp"
#include "font.cpp"
#include "window.cpp"
#include "layout.cpp"
#include "message-window.cpp"

#include "action/action.cpp"
#include "action/menu.cpp"
#include "action/menu-separator.cpp"
#include "action/menu-item.cpp"
#include "action/menu-check-item.cpp"
#include "action/menu-radio-item.cpp"

#include "widget/widget.cpp"
#include "widget/button.cpp"
#include "widget/canvas.cpp"
#include "widget/check-box.cpp"
#include "widget/combo-box.cpp"
#include "widget/edit-box.cpp"
#include "widget/hex-editor.cpp"
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/list-box.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-box.cpp"
#include "widget/text-box.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

OS::Data *OS::os = 0;
Window Window::None;

void OS::initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

  settings.load();

  os = new OS::Data;
  static int argc = 1;
  static char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;
  os->application = new QApplication(argc, argvp);
}

bool OS::pending() {
  return QApplication::hasPendingEvents();
}

void OS::run() {
  QApplication::processEvents();
}

void OS::main() {
  QApplication::exec();
}

void OS::quit() {
  settings.save();
  QApplication::quit();
}

unsigned OS::desktopWidth() {
  return QApplication::desktop()->screenGeometry().width();
}

unsigned OS::desktopHeight() {
  return QApplication::desktop()->screenGeometry().height();
}

string OS::folderSelect(Window &parent, const string &path) {
  QString directory = QFileDialog::getExistingDirectory(
    &parent != &Window::None ? parent.window : 0, "Select Directory",
    QString::fromUtf8(path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name.endswith("/") == false) name.append("/");
  return name;
}

string OS::fileOpen(Window &parent, const string &filter, const string &path) {
  string filters;
  lstring list;
  list.split("\n", filter);
  foreach(item, list) {
    lstring part;
    part.split("\t", item);
    if(part.size() != 2) continue;
    part[1].replace(",", " ");
    filters.append(part[0]);
    filters.append(" (");
    filters.append(part[1]);
    filters.append(");;");
  }
  filters.rtrim(";;");

  QString filename = QFileDialog::getOpenFileName(
    &parent != &Window::None ? parent.window : 0, "Open File",
    QString::fromUtf8(path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

string OS::fileSave(Window &parent, const string &filter, const string &path) {
  string filters;
  lstring list;
  list.split("\n", filter);
  foreach(item, list) {
    lstring part;
    part.split("\t", item);
    if(part.size() != 2) continue;
    part[1].replace(",", " ");
    filters.append(part[0]);
    filters.append(" (");
    filters.append(part[1]);
    filters.append(");;");
  }
  filters.rtrim(";;");

  QString filename = QFileDialog::getSaveFileName(
    &parent != &Window::None ? parent.window : 0, "Save File",
    QString::fromUtf8(path), QString::fromUtf8(filters)
  );
  return filename.toUtf8().constData();
}

}
