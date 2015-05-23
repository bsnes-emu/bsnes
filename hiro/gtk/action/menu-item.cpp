#if defined(Hiro_MenuItem)

namespace hiro {

static auto MenuItem_activate(GtkMenuItem*, pMenuItem* p) -> void {
  p->self().doActivate();
}

auto pMenuItem::construct() -> void {
  widget = gtk_image_menu_item_new_with_mnemonic("");
  g_signal_connect(G_OBJECT(widget), "activate", G_CALLBACK(MenuItem_activate), (gpointer)this);
  setText(state().text);
}

auto pMenuItem::destruct() -> void {
  if(widget) gtk_widget_destroy(widget), widget = nullptr;
}

auto pMenuItem::setIcon(const image& icon) -> void {
  if(icon) {
    GtkImage* gtkImage = CreateImage(icon, true);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), (GtkWidget*)gtkImage);
  } else {
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), nullptr);
  }
}

auto pMenuItem::setText(const string& text) -> void {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), _mnemonic(text));
}

}

#endif
