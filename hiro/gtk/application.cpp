#if defined(Hiro_Application)

namespace hiro {

#if defined(DISPLAY_XORG)
XlibDisplay* pApplication::display = nullptr;
#endif

auto pApplication::run() -> void {
  if(Application::state.onMain) {
    while(!Application::state.quit) {
      Application::doMain();
      processEvents();
    }
  } else {
    gtk_main();
  }
}

auto pApplication::pendingEvents() -> bool {
  return gtk_events_pending();
}

auto pApplication::processEvents() -> void {
  while(pendingEvents()) gtk_main_iteration_do(false);
}

auto pApplication::quit() -> void {
  //if gtk_main() was invoked, call gtk_main_quit()
  if(gtk_main_level()) gtk_main_quit();

  #if defined(DISPLAY_XORG)
  //todo: Keyboard::poll() is being called after Application::quit();
  //so if display is closed; this causes a segfault
  //XCloseDisplay(display);
  //display = nullptr;
  #endif
}

auto pApplication::initialize() -> void {
  #if defined(DISPLAY_XORG)
  display = XOpenDisplay(nullptr);
  #endif

  //set WM_CLASS to Application::name()
  auto name = Application::state.name ? Application::state.name : string{"hiro"};
  gdk_set_program_class(name);

  #if 1
  int argc = 1;
  char* argv[] = {name.get(), nullptr};
  #else
  //--g-fatal-warnings will force a trap on Gtk-CRITICAL and Gtk-WARNING messages
  //this allows gdb to perform a backtrace to find an error's origin point
  int argc = 2;
  char* argv[] = {name.get(), new char[19], nullptr};
  strcpy(argv[1], "--g-fatal-warnings");
  #endif
  char** argvp = argv;

  gtk_init(&argc, &argvp);
  GtkSettings* gtkSettings = gtk_settings_get_default();

  //allow buttons to show icons
  g_type_class_unref(g_type_class_ref(GTK_TYPE_BUTTON));
  g_object_set(gtkSettings, "gtk-button-images", true, nullptr);

  #if defined(DISPLAY_WINDOWS)
  //there is a serious bug in GTK 2.24 for Windows with the "ime" (Windows IME) input method:
  //by default, it will be impossible to type in text fields at all.
  //there are various tricks to get around this; but they are unintuitive and unreliable.
  //the "ime" method is chosen when various international system locales (eg Japanese) are selected.
  //here, we override the default input method to use the "Simple" type instead to avoid the bug.
  //obviously, this has a drawback: in-place editing for IMEs will not work in this mode.
  g_object_set(gtkSettings, "gtk-im-module", "gtk-im-context-simple", nullptr);
  #endif

  #if HIRO_GTK==2
  gtk_rc_parse_string(R"(
    style "HiroWindow"
    {
      GtkWindow::resize-grip-width = 0
      GtkWindow::resize-grip-height = 0
    }
    class "GtkWindow" style "HiroWindow"

    style "HiroTreeView"
    {
      GtkTreeView::vertical-separator = 0
    }
    class "GtkTreeView" style "HiroTreeView"

    style "HiroTabFrameCloseButton"
    {
      GtkWidget::focus-line-width = 0
      GtkWidget::focus-padding = 0
      GtkButton::default-border = {0, 0, 0, 0}
      GtkButton::default-outer-border = {0, 0, 0, 0}
      GtkButton::inner-border = {0, 1, 0, 0}
    }
    widget_class "*.<GtkNotebook>.<GtkHBox>.<GtkButton>" style "HiroTabFrameCloseButton"
  )");
  #elif HIRO_GTK==3
  //TODO: is there any alternative here with GTK3?
  #endif

  pKeyboard::initialize();
}

}

#endif
