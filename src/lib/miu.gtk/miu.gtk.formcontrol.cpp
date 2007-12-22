void pFormControl::resize(uint width, uint height) {
  gtk_widget_set_size_request(gtk_handle(), width, height);
}

void pFormControl::focus() {
  gtk_widget_grab_focus(gtk_handle());
}

bool pFormControl::focused() {
  return GTK_WIDGET_HAS_FOCUS(gtk_handle());
}

void pFormControl::enable(bool state) {
  gtk_widget_set_sensitive(gtk_handle(), state);
}

void pFormControl::disable() {
  enable(false);
}

bool pFormControl::enabled() {
  return GTK_WIDGET_SENSITIVE(gtk_handle());
}

pFormControl::pFormControl(FormControl &self_) : pWidget(self_), self(self_) {
}

/* internal */

GtkWidget* pFormControl::gtk_handle() {
  return 0;
}
