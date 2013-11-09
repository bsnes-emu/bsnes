namespace phoenix {

XlibDisplay* pApplication::display = nullptr;

void pApplication::run() {
  if(Application::main) {
    while(applicationState.quit == false) {
      processEvents();
      Application::main();
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

  int argc = 1;
  char* argv[] = {new char[8], nullptr};
  strcpy(argv[0], "phoenix");
  char** argvp = argv;
  gtk_init(&argc, &argvp);

  GtkSettings* gtkSettings = gtk_settings_get_default();
//g_object_set(gtkSettings, "gtk-button-images", true, nullptr);

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

}
