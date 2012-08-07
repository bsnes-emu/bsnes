#include "platform.hpp"

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

void pOS::main() {
}

bool pOS::pendingEvents() {
  return false;
}

void pOS::processEvents() {
}

void pOS::quit() {
}

void pOS::initialize() {
}
