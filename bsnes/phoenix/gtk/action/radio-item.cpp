static void RadioItem_tick(RadioItem *self) {
  foreach(item, self->state.group) item.state.checked = (&item == self);
  if(self->p.locked == false && self->checked() && self->onTick) self->onTick();
}

bool pRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
}

void pRadioItem::setChecked() {
  locked = true;
  foreach(item, radioItem.state.group) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item.p.widget), false);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), true);
  locked = false;
}

void pRadioItem::setGroup(const reference_array<RadioItem&> &group) {
  foreach(item, group, n) {
    if(n == 0) continue;
    GSList *currentGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(group[0].p.widget));
    if(currentGroup != gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item.p.widget))) {
      gtk_radio_menu_item_set_group(GTK_RADIO_MENU_ITEM(item.p.widget), currentGroup);
    }
  }
}

void pRadioItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pRadioItem::constructor() {
  widget = gtk_radio_menu_item_new_with_label(0, radioItem.state.text);
  setGroup(radioItem.state.group);
  foreach(item, radioItem.state.group, n) {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item.p.widget), item.state.checked);
  }
  g_signal_connect_swapped(G_OBJECT(widget), "toggled", G_CALLBACK(RadioItem_tick), (gpointer)&radioItem);
}

void pRadioItem::destructor() {
  gtk_widget_destroy(widget);
}

void pRadioItem::orphan() {
  destructor();
  constructor();
}
