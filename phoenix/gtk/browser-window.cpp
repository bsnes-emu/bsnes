namespace phoenix {

static void BrowserWindow_addFilters(GtkWidget* dialog, lstring filters) {
  for(auto& filter : filters) {
    GtkFileFilter* gtkFilter = gtk_file_filter_new();
    gtk_file_filter_set_name(gtkFilter, filter);
    lstring patterns = filter.split<1>("(")(1).rtrim<1>(")").split(",");
    for(auto& pattern : patterns) gtk_file_filter_add_pattern(gtkFilter, pattern.strip());
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gtkFilter);
  }
}

string pBrowserWindow::directory(BrowserWindow::State& state) {
  string name;

  GtkWidget* dialog = gtk_file_chooser_dialog_new(
    state.title ? state.title : "Select Directory",
    state.parent ? GTK_WINDOW(state.parent->p.widget) : (GtkWindow*)nullptr,
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)nullptr
  );

  if(state.path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), state.path);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char* temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  if(name && !name.endsWith("/")) name.append("/");
  return name;
}

string pBrowserWindow::open(BrowserWindow::State& state) {
  string name;

  GtkWidget* dialog = gtk_file_chooser_dialog_new(
    state.title ? state.title : "Open File",
    state.parent ? GTK_WINDOW(state.parent->p.widget) : (GtkWindow*)nullptr,
    GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)nullptr
  );

  if(state.path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), state.path);
  BrowserWindow_addFilters(dialog, state.filters);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char* temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  return name;
}

string pBrowserWindow::save(BrowserWindow::State& state) {
  string name;

  GtkWidget* dialog = gtk_file_chooser_dialog_new(
    state.title ? state.title : "Save File",
    state.parent ? GTK_WINDOW(state.parent->p.widget) : (GtkWindow*)nullptr,
    GTK_FILE_CHOOSER_ACTION_SAVE,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)nullptr
  );

  if(state.path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), state.path);
  BrowserWindow_addFilters(dialog, state.filters);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char* temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  return name;
}

}
