void pWidget::show(bool state) {
  state ? gtk_widget_show(gtk_handle()) : gtk_widget_hide(gtk_handle());
}

void pWidget::hide() {
  show(false);
}

bool pWidget::visible() {
  return GTK_WIDGET_VISIBLE(gtk_handle());
}

uintptr_t pWidget::handle() {
  return GDK_WINDOW_XID(gtk_handle()->window);
}

pWidget::pWidget(Widget &self_) : self(self_) {
}

/* internal */

GtkWidget* pWidget::gtk_handle() {
  return 0;
}
