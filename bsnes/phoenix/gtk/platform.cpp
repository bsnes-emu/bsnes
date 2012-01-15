#include "platform.hpp"
#include "utility.cpp"

#include "desktop.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"
#include "dialog-window.cpp"
#include "message-window.cpp"

#include "settings.cpp"
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
Font pOS::defaultFont;

void pOS::main() {
  gtk_main();
}

bool pOS::pendingEvents() {
  return gtk_events_pending();
}

void pOS::processEvents() {
  while(pendingEvents()) gtk_main_iteration_do(false);
}

void pOS::quit() {
  gtk_main_quit();
}

void pOS::initialize() {
  display = XOpenDisplay(0);

  settings = new Settings;
  settings->load();

  int argc = 1;
  char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;
  gtk_init(&argc, &argvp);

  gtk_rc_parse_string(R"(
    style "phoenix-gtk"
    {
      GtkWindow::resize-grip-width = 0
      GtkWindow::resize-grip-height = 0
      GtkTreeView::vertical-separator = 0
      GtkComboBox::appears-as-list = 1
    }
    class "GtkWindow" style "phoenix-gtk"
    class "GtkTreeView" style "phoenix-gtk"
  # class "GtkComboBox" style "phoenix-gtk"
  )");

  pKeyboard::initialize();
}
