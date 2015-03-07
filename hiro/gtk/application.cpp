namespace hiro {

#if defined(PLATFORM_XORG)
XlibDisplay* pApplication::display = nullptr;
#endif

void pApplication::run() {
  if(Application::state.onMain) {
    while(Application::state.quit == false) {
      processEvents();
      Application::doMain();
    }
  } else {
    gtk_main();
  }
}

bool pApplication::pendingEvents() {
  return gtk_events_pending();
}

void pApplication::processEvents() {
  while(pendingEvents()) gtk_main_iteration_do(false);
}

void pApplication::quit() {
  //if gtk_main() was invoked, call gtk_main_quit()
  if(gtk_main_level()) gtk_main_quit();

  #if defined(PLATFORM_XORG)
  //todo: Keyboard::poll() is being called after Application::quit();
  //so if display is closed; this causes a segfault
  //XCloseDisplay(display);
  //display = nullptr;
  #endif
}

void pApplication::initialize() {
  #if defined(PLATFORM_XORG)
  display = XOpenDisplay(nullptr);
  #endif

  settings = new Settings;
  settings->load();

  //set WM_CLASS to Application::name()
  if(Application::state.name) gdk_set_program_class(Application::state.name);

  #if 1
  int argc = 1;
  char* argv[] = {new char[5], nullptr};
  strcpy(argv[0], "hiro");
  #else
  //--g-fatal-warnings will force a trap on Gtk-CRITICAL errors
  //this allows gdb to perform a backtrace to find an error's origin point
  int argc = 2;
  char* argv[] = {new char[5], new char[19], nullptr};
  strcpy(argv[0], "hiro");
  strcpy(argv[1], "--g-fatal-warnings");
  #endif
  char** argvp = argv;

  gtk_init(&argc, &argvp);
  GtkSettings* gtkSettings = gtk_settings_get_default();

  //allow buttons to show icons
  g_type_class_unref(g_type_class_ref(GTK_TYPE_BUTTON));
  g_object_set(gtkSettings, "gtk-button-images", true, nullptr);

  #if defined(PLATFORM_WINDOWS)
  //there is a serious bug in GTK 2.24 for Windows with the "ime" (Windows IME) input method:
  //by default, it will be impossible to type in text fields at all.
  //there are various tricks to get around this; but they are unintuitive and unreliable.
  //the "ime" method is chosen when various international system locales (eg Japanese) are selected.
  //here, we override the default input method to use the "Simple" type instead to avoid the bug.
  //obviously, this has a drawback: in-place editing for IMEs will not work in this mode.
  g_object_set(gtkSettings, "gtk-im-module", "gtk-im-context-simple", nullptr);
  #endif

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

  pKeyboard::initialize();
}

}
