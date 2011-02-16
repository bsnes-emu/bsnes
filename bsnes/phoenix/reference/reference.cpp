#include "reference.hpp"

#include "font.cpp"
#include "message-window.cpp"
#include "window.cpp"
#include "layout.cpp"

#include "action/action.cpp"
#include "action/menu.cpp"
#include "action/menu-separator.cpp"
#include "action/menu-item.cpp"
#include "action/menu-check-item.cpp"
#include "action/menu-radio-item.cpp"

#include "widget/widget.cpp"
#include "widget/button.cpp"
#include "widget/check-box.cpp"
#include "widget/combo-box.cpp"
#include "widget/hex-edit.cpp"
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/line-edit.cpp"
#include "widget/list-view.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-box.cpp"
#include "widget/text-edit.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

unsigned pOS::desktopWidth() {
  return 0;
}

unsigned pOS::desktopHeight() {
  return 0;
}

string pOS::fileLoad(Window &parent, const string &path, const lstring &filter) {
  return "";
}

string pOS::fileSave(Window &parent, const string &path, const lstring &filter) {
  return "";
}

string pOS::folderSelect(Window &parent, const string &path) {
  return "";
}

void pOS::main() {
}

bool pOS::pending() {
  return false;
}

void pOS::process() {
}

void pOS::quit() {
}

void pOS::initialize() {
}
