#include <QApplication>
#include <QtGui>
using namespace nall;

namespace phoenix {

#include "qt.moc.hpp"
#include "qt.moc"

#include "object.cpp"
#include "font.cpp"
#include "menu.cpp"
#include "window.cpp"
#include "widget.cpp"
#include "layout.cpp"
#include "fixed-layout.cpp"
#include "button.cpp"
#include "canvas.cpp"
#include "checkbox.cpp"
#include "combobox.cpp"
#include "editbox.cpp"
#include "hexeditor.cpp"
#include "horizontalslider.cpp"
#include "label.cpp"
#include "listbox.cpp"
#include "progressbar.cpp"
#include "radiobox.cpp"
#include "textbox.cpp"
#include "verticalslider.cpp"
#include "viewport.cpp"
#include "messagewindow.cpp"

OS::Data *OS::os = 0;
Window Window::None;

void OS::initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

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
