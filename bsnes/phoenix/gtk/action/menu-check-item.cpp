static void MenuCheckItem_tick(MenuCheckItem *self) {
  if(self->p.locked == false && self->onTick) self->onTick();
}

bool pMenuCheckItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

void pMenuCheckItem::setChecked(bool checked) {
  locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), checked);
  locked = false;
}

void pMenuCheckItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pMenuCheckItem::constructor() {
  widget = gtk_check_menu_item_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(widget), "toggled", G_CALLBACK(MenuCheckItem_tick), (gpointer)&menuCheckItem);
}
