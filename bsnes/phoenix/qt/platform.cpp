#include "platform.moc.hpp"
#include "platform.moc"
#include "utility.cpp"
#include "settings.cpp"

#include "desktop.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"
#include "dialog-window.cpp"
#include "message-window.cpp"

#include "font.cpp"
#include "timer.cpp"
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

XlibDisplay* pOS::display = 0;

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
  QApplication::quit();
  //note: QApplication cannot be deleted; or libQtGui will crash
  qtApplication = 0;
}

void pOS::syncX() {
  for(unsigned n = 0; n < 8; n++) {
    QApplication::syncX();
    OS::processEvents();
    usleep(2000);
  }
}

void pOS::initialize() {
  display = XOpenDisplay(0);

  settings = new Settings;
  settings->load();

  static int argc = 1;
  static char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;

  qtApplication = new QApplication(argc, argvp);

  pKeyboard::initialize();
}
