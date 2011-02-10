static void MenuItem_tick(MenuItem *self) {
  if(self->onTick) self->onTick();
}

void MenuItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(object->widget), text);
}

MenuItem::MenuItem() {
  object->widget = gtk_menu_item_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(object->widget), "activate", G_CALLBACK(MenuItem_tick), (gpointer)this);
}
