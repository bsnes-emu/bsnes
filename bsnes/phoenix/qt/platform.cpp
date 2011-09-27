#include "platform.moc.hpp"
#include "platform.moc"

#include "settings.cpp"
#include "font.cpp"
#include "timer.cpp"
#include "message-window.cpp"
#include "window.cpp"

#include "action/action.cpp"
#include "action/menu.cpp"
#include "action/separator.cpp"
#include "action/item.cpp"
#include "action/check-item.cpp"
#include "action/radio-item.cpp"

#include "widget/widget.cpp"
#include "widget/button.cpp"
#include "widget/canvas.cpp"
#include "widget/check-box.cpp"
#include "widget/combo-box.cpp"
#include "widget/hex-edit.cpp"
#include "widget/horizontal-scroll-bar.cpp"
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/line-edit.cpp"
#include "widget/list-view.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-box.cpp"
#include "widget/text-edit.cpp"
#include "widget/vertical-scroll-bar.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

Geometry pOS::availableGeometry() {
  QRect rect = QApplication::desktop()->availableGeometry();
  return { rect.x(), rect.y(), rect.width(), rect.height() };
}

Geometry pOS::desktopGeometry() {
  QRect rect = QApplication::desktop()->screenGeometry();
  return { 0, 0, rect.width(), rect.height() };
}

string pOS::fileLoad(Window &parent, const string &path, const lstring &filter) {
  string filterList;
  for(auto &item : filter) {
    filterList.append(item);
    filterList.append(";;");
  }
  filterList.rtrim<1>(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parenthesis = 0;
  for(auto &n : filterList) {
    if(n == '(') parenthesis++;
    if(n == ')') parenthesis--;
    if(n == ',' && parenthesis) n = ' ';
  }

  QString filename = QFileDialog::getOpenFileName(
    &parent != &Window::None ? parent.p.qtWindow : 0, "Load File",
    QString::fromUtf8(path), QString::fromUtf8(filterList)
  );
  return filename.toUtf8().constData();
}

string pOS::fileSave(Window &parent, const string &path, const lstring &filter) {
  string filterList;
  for(auto &item : filter) {
    filterList.append(item);
    filterList.append(";;");
  }
  filterList.rtrim<1>(";;");

  //convert filter list from phoenix to Qt format, example:
  //"Text, XML files (*.txt,*.xml)" -> "Text, XML files (*.txt *.xml)"
  signed parenthesis = 0;
  for(auto &n : filterList) {
    if(n == '(') parenthesis++;
    if(n == ')') parenthesis--;
    if(n == ',' && parenthesis) n = ' ';
  }

  QString filename = QFileDialog::getSaveFileName(
    &parent != &Window::None ? parent.p.qtWindow : 0, "Save File",
    QString::fromUtf8(path), QString::fromUtf8(filterList)
  );
  return filename.toUtf8().constData();
}

string pOS::folderSelect(Window &parent, const string &path) {
  QString directory = QFileDialog::getExistingDirectory(
    &parent != &Window::None ? parent.p.qtWindow : 0, "Select Directory",
    QString::fromUtf8(path), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );
  string name = directory.toUtf8().constData();
  if(name != "" && name.endswith("/") == false) name.append("/");
  return name;
}

void pOS::main() {
  QApplication::exec();
}

bool pOS::pendingEvents() {
  return QApplication::hasPendingEvents();
}

void pOS::processEvents() {
  while(pendingEvents()) QApplication::processEvents();
}

void pOS::quit() {
  settings->save();

  QApplication::quit();
  //note: QApplication cannot be deleted; or libQtGui will crash
  qtApplication = 0;
}

void pOS::initialize() {
  settings = new Settings;
  settings->load();

  static int argc = 1;
  static char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;

  qtApplication = new QApplication(argc, argvp);
}
