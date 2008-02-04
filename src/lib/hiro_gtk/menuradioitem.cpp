void hiro_pmenuradioitem_tick(pMenuRadioItem *p) {
//GTK+ sends two messages: one for the activated radio item,
//and one for the deactivated radio item. ignore the latter.
  if(p->checked() && p->self.on_tick) p->self.on_tick(Event(Event::Tick, 0, &p->self));
}

void pMenuRadioItem::create(MenuRadioItemGroup &group, const char *text) {
  if(group.size() == 0 || group[0] == &self) {
    item = gtk_radio_menu_item_new_with_label(0, text ? text : "?");
  } else {
    item = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(group[0]->p.gtk_handle()), text ? text : "");
  }
  gtk_widget_show(item);
  g_signal_connect_swapped(G_OBJECT(item), "toggled", G_CALLBACK(hiro_pmenuradioitem_tick), (gpointer)this);
}

void pMenuRadioItem::check() {
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
}

bool pMenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(item));
}

pMenuRadioItem::pMenuRadioItem(MenuRadioItem &self_) : pMenuControl(self_), self(self_) {
  item = 0;
}

/* internal */

GtkWidget* pMenuRadioItem::gtk_handle() {
  return item;
}
