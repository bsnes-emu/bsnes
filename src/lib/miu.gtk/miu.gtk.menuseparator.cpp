void pMenuSeparator::create() {
  item = gtk_separator_menu_item_new();
  gtk_widget_show(item);
}

pMenuSeparator::pMenuSeparator(MenuSeparator &self_) : pMenuControl(self_), self(self_) {
  item = 0;
}

/* internal */

GtkWidget* pMenuSeparator::gtk_handle() {
  return item;
}
