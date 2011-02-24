static void MenuRadioItem_tick(MenuRadioItem *self) {
  if(self->p.locked == false && self->checked() && self->onTick) self->onTick();
}

bool pMenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

void pMenuRadioItem::setChecked() {
  locked = true;
  foreach(item, menuRadioItem.state.group) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item.p.widget), false);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), true);
  locked = false;
}

void pMenuRadioItem::setGroup(const reference_array<MenuRadioItem&> &group) {
  foreach(item, group, n) {
    if(n == 0) continue;
    GSList *currentGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(group[0].p.widget));
    if(currentGroup != gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item.p.widget))) {
      gtk_radio_menu_item_set_group(GTK_RADIO_MENU_ITEM(item.p.widget), currentGroup);
    }
  }
}

void pMenuRadioItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pMenuRadioItem::constructor() {
  widget = gtk_radio_menu_item_new_with_label(0, "");
  g_signal_connect_swapped(G_OBJECT(widget), "toggled", G_CALLBACK(MenuRadioItem_tick), (gpointer)&menuRadioItem);
}
