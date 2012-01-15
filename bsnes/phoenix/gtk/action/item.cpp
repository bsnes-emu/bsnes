static void Item_activate(Item *self) {
  if(self->onActivate) self->onActivate();
}

void pItem::setImage(const image &image) {
  GtkImage *gtkImage = CreateImage(image, /* menuIcon = */ true);
  gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), (GtkWidget*)gtkImage);
}

void pItem::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pItem::constructor() {
  widget = gtk_image_menu_item_new_with_label(item.state.text);
  g_signal_connect_swapped(G_OBJECT(widget), "activate", G_CALLBACK(Item_activate), (gpointer)&item);
}

void pItem::destructor() {
  gtk_widget_destroy(widget);
}

void pItem::orphan() {
  destructor();
  constructor();
}
