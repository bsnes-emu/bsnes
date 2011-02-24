static void MenuItem_tick(MenuItem *self) {
  if(self->onTick) self->onTick();
}

void pMenuItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pMenuItem::constructor() {
  widget = gtk_menu_item_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(widget), "activate", G_CALLBACK(MenuItem_tick), (gpointer)&menuItem);
}
