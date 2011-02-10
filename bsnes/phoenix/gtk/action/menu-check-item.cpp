static void MenuCheckItem_tick(MenuCheckItem *self) {
  if(self->onTick && self->object->locked == false) self->onTick();
}

void MenuCheckItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(object->widget), text);
}

bool MenuCheckItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(object->widget));
}

void MenuCheckItem::setChecked(bool state) {
  object->locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(object->widget), state);
  object->locked = false;
}

MenuCheckItem::MenuCheckItem() {
  object->widget = gtk_check_menu_item_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(MenuCheckItem_tick), (gpointer)this);
}
