#include <QApplication>
#include <QtGui>
using namespace nall;

namespace phoenix {

#include "qt.moc.hpp"
#include "qt.moc"

#include "object.cpp"
#include "font.cpp"
#include "menu.cpp"
#include "widget.cpp"
#include "window.cpp"
#include "button.cpp"
#include "canvas.cpp"
#include "checkbox.cpp"
#include "combobox.cpp"
#include "editbox.cpp"
#include "horizontalslider.cpp"
#include "label.cpp"
#include "listbox.cpp"
#include "progressbar.cpp"
#include "radiobox.cpp"
#include "textbox.cpp"
#include "verticalslider.cpp"
#include "viewport.cpp"
#include "messagewindow.cpp"

OS &os = OS::handle();
Window Window::None;

OS& OS::handle() {
  static OS os;
  return os;
}

bool OS::pending() {
  return QApplication::hasPendingEvents();
}

bool OS::run() {
  QApplication::processEvents();
  return QApplication::hasPendingEvents();
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

string OS::folderSelect(Window &parent, const char *path) {
  QString directory = QFileDialog::getExistingDirectory(
    &parent != &Window::None ? parent.window : 0, "Select Directory", path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  return directory.toUtf8().constData();
}

string OS::fileOpen(Window &parent, const char *filter, const char *path) {
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
    &parent != &Window::None ? parent.window : 0, "Open File", path, (const char*)filters
  );
  return filename.toUtf8().constData();
}

string OS::fileSave(Window &parent, const char *filter, const char *path) {
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
    &parent != &Window::None ? parent.window : 0, "Save File", path, (const char*)filters
  );
  return filename.toUtf8().constData();
}

OS::OS() {
  os = new OS::Data(*this);
  static int argc = 1;
  static char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;
  os->application = new QApplication(argc, argvp);
}

}
