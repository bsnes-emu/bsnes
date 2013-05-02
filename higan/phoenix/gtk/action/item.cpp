namespace phoenix {

static void Item_activate(Item* self) {
  if(self->onActivate) self->onActivate();
}

void pItem::setImage(const image& image) {
  if(image.empty() == false) {
    GtkImage* gtkImage = CreateImage(image, true);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), (GtkWidget*)gtkImage);
  } else {
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), nullptr);
  }
}

void pItem::setText(const string& text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), mnemonic(text));
}

void pItem::constructor() {
  widget = gtk_image_menu_item_new_with_mnemonic("");
  g_signal_connect_swapped(G_OBJECT(widget), "activate", G_CALLBACK(Item_activate), (gpointer)&item);
  setText(item.state.text);
}

void pItem::destructor() {
  gtk_widget_destroy(widget);
}

void pItem::orphan() {
  destructor();
  constructor();
}

}
