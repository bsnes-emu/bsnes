void pSeparator::constructor() {
  widget = gtk_separator_menu_item_new();
}

void pSeparator::destructor() {
  gtk_widget_destroy(widget);
}

void pSeparator::orphan() {
  destructor();
  constructor();
}
