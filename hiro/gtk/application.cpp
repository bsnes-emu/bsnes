namespace hiro {

XlibDisplay* pApplication::display = nullptr;

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
}

void pApplication::initialize() {
  display = XOpenDisplay(nullptr);

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
  g_object_set(gtkSettings, "gtk-button-images", true, nullptr);

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
