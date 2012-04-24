static string FileDialog(bool save, Window &parent, const string &path, const lstring &filter) {
  string name;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    save == 0 ? "Load File" : "Save File",
    &parent != &Window::None ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)0,
    save == 0 ? GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0
  );

  if(path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  for(auto &filterItem : filter) {
    GtkFileFilter *gtkFilter = gtk_file_filter_new();
    gtk_file_filter_set_name(gtkFilter, filterItem);
    lstring part;
    part.split("(", filterItem);
    part[1].rtrim<1>(")");
    lstring list;
    list.split(",", part[1]);
    for(auto &pattern : list) gtk_file_filter_add_pattern(gtkFilter, pattern);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gtkFilter);
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  return name;
}

string pDialogWindow::fileOpen(Window &parent, const string &path, const lstring &filter) {
  return FileDialog(0, parent, path, filter);
}

string pDialogWindow::fileSave(Window &parent, const string &path, const lstring &filter) {
  return FileDialog(1, parent, path, filter);
}

string pDialogWindow::folderSelect(Window &parent, const string &path) {
  string name;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "Select Folder",
    &parent != &Window::None ? GTK_WINDOW(parent.p.widget) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0
  );

  if(path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  if(name == "") return "";
  if(name.endswith("/") == false) name.append("/");
  return name;
}
