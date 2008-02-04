void pMenuControl::enable(bool state) {
  gtk_widget_set_sensitive(gtk_handle(), state);
}

void pMenuControl::disable() {
  enable(false);
}

bool pMenuControl::enabled() {
  return GTK_WIDGET_SENSITIVE(gtk_handle());
}

pMenuControl::pMenuControl(MenuControl &self_) : pWidget(self_), self(self_) {
}

/* internal */

GtkWidget* pMenuControl::gtk_handle() {
  return 0;
}
