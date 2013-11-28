namespace phoenix {

static void RadioItem_activate(GtkCheckMenuItem* gtkCheckMenuItem, RadioItem* self) {
  self->p.onActivate();
}

void pRadioItem::setChecked() {
  parent().locked = true;
  for(auto& item : radioItem.state.group) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item.p.widget), false);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), true);
  parent().locked = false;
}

void pRadioItem::setGroup(const group<RadioItem>& group) {
  parent().locked = true;
  for(auto& item : group) {
    if(&item == &group.first()) continue;
    GSList* currentGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(group.first().p.widget));
    if(currentGroup != gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item.p.widget))) {
      gtk_radio_menu_item_set_group(GTK_RADIO_MENU_ITEM(item.p.widget), currentGroup);
    }
  }
  parent().locked = false;
}

void pRadioItem::setText(string text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), mnemonic(text));
}

void pRadioItem::constructor() {
  widget = gtk_radio_menu_item_new_with_mnemonic(0, "");
  setGroup(radioItem.state.group);
  setText(radioItem.state.text);
  for(auto& item : radioItem.state.group) {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item.p.widget), item.state.checked);
  }
  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(RadioItem_activate), (gpointer)&radioItem);
}

void pRadioItem::destructor() {
  gtk_widget_destroy(widget);
}

void pRadioItem::orphan() {
  destructor();
  constructor();
}

void pRadioItem::onActivate() {
  if(parent().locked) return;
  bool wasChecked = radioItem.state.checked;
  radioItem.setChecked();
  if(wasChecked) return;
  if(radioItem.onActivate) radioItem.onActivate();
}

pRadioItem& pRadioItem::parent() {
  if(radioItem.state.group.size()) return radioItem.state.group.first().p;
  return *this;
}

}
