void miu_pmenucheckitem_tick(pMenuCheckItem *p) {
  if(p->self.on_tick) p->self.on_tick(Event(Event::Tick, p->checked(), &p->self));
}

void pMenuCheckItem::create(const char *text) {
  item = gtk_check_menu_item_new_with_label(text ? text : "?");
  gtk_widget_show(item);
  g_signal_connect_swapped(G_OBJECT(item), "toggled", G_CALLBACK(miu_pmenucheckitem_tick), (gpointer)this);
}

void pMenuCheckItem::check(bool state) {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), state ? TRUE : FALSE);
}

void pMenuCheckItem::uncheck() {
  check(false);
}

bool pMenuCheckItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(item));
}

pMenuCheckItem::pMenuCheckItem(MenuCheckItem &self_) : pMenuControl(self_), self(self_) {
  item = 0;
}

/* internal */

GtkWidget* pMenuCheckItem::gtk_handle() {
  return item;
}
