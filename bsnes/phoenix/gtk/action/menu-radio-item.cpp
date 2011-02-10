static void MenuRadioItem_tick(MenuRadioItem *self) {
  if(self->onTick && self->checked() && self->object->locked == false) self->onTick();
}

void MenuRadioItem::setParent(MenuRadioItem &parent) {
  gtk_radio_menu_item_set_group(
    GTK_RADIO_MENU_ITEM(object->widget),
    gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(parent.object->widget))
  );
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(object->widget), false);
}

void MenuRadioItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(object->widget), text);
}

bool MenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(object->widget));
}

void MenuRadioItem::setChecked() {
  object->locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(object->widget), true);
  object->locked = false;
}

MenuRadioItem::MenuRadioItem() {
  object->widget = gtk_radio_menu_item_new_with_label(0, "");
  setChecked();
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(MenuRadioItem_tick), (gpointer)this);
}
