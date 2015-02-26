namespace hiro {

static auto MenuCheckItem_toggle(GtkCheckMenuItem* gtkCheckMenuItem, pMenuCheckItem* p) -> void {
  p->state().checked = gtk_check_menu_item_get_active(gtkCheckMenuItem);
  if(!p->locked()) p->self().doToggle();
}

auto pMenuCheckItem::construct() -> void {
  widget = gtk_check_menu_item_new_with_mnemonic("");
  setChecked(state().checked);
  setText(state().text);
  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(MenuCheckItem_toggle), (gpointer)this);
}

auto pMenuCheckItem::destruct() -> void {
  gtk_widget_destroy(widget);
}

auto pMenuCheckItem::orphan() -> void {
  destruct();
  construct();
}

auto pMenuCheckItem::setChecked(bool checked) -> void {
  lock();
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), checked);
  unlock();
}

auto pMenuCheckItem::setText(const string& text) -> void {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), _mnemonic(text));
}

}
