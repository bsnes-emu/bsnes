void miu_pmenuitem_tick(pMenuItem *p) {
  if(p->self.on_tick) p->self.on_tick(Event(Event::Tick, 0, &p->self));
}

void pMenuItem::create(const char *text) {
  item = gtk_menu_item_new_with_label(text ? text : "");
  g_signal_connect_swapped(G_OBJECT(item), "activate", G_CALLBACK(miu_pmenuitem_tick), (gpointer)this);
  gtk_widget_show(item);
}

pMenuItem::pMenuItem(MenuItem &self_) : pMenuControl(self_), self(self_) {
  item = 0;
}

/* internal */

GtkWidget* pMenuItem::gtk_handle() {
  return item;
}
