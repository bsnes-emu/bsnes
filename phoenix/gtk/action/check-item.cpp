namespace phoenix {

static void CheckItem_toggle(GtkCheckMenuItem* gtkCheckMenuItem, CheckItem* self) {
  self->state.checked = gtk_check_menu_item_get_active(gtkCheckMenuItem);
  if(!self->p.locked && self->onToggle) self->onToggle();
}

void pCheckItem::setChecked(bool checked) {
  locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), checked);
  locked = false;
}

void pCheckItem::setText(string text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), mnemonic(text));
}

void pCheckItem::constructor() {
  widget = gtk_check_menu_item_new_with_mnemonic("");
  setChecked(checkItem.state.checked);
  setText(checkItem.state.text);
  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(CheckItem_toggle), (gpointer)&checkItem);
}

void pCheckItem::destructor() {
  gtk_widget_destroy(widget);
}

void pCheckItem::orphan() {
  destructor();
  constructor();
}

}
