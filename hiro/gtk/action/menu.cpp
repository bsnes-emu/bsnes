#if defined(Hiro_Menu)

namespace hiro {

auto pMenu::construct() -> void {
  gtkMenu = gtk_menu_new();
  widget = gtk_image_menu_item_new_with_mnemonic("");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget), gtkMenu);
  setIcon(state().icon);
  setText(state().text);

  for(auto& action : state().actions) append(*action);
}

auto pMenu::destruct() -> void {
  gtk_widget_destroy(gtkMenu);
  gtk_widget_destroy(widget);
}

auto pMenu::append(sAction action) -> void {
  if(action->self()) {
    gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), action->self()->widget);
    action->self()->setFont(action->font(true));
    action->self()->setVisible(action->visible());  //hidden parent will hide child; no need to inherit visibility
  }
}

auto pMenu::remove(sAction action) -> void {
}

auto pMenu::setFont(const Font& font) -> void {
  pAction::setFont(font);
  for(auto& action : state().actions) {
    if(action->self()) action->self()->setFont(action->font(true));
  }
}

auto pMenu::setIcon(const image& icon) -> void {
  if(icon) {
    GtkImage* gtkImage = CreateImage(icon, true);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), (GtkWidget*)gtkImage);
  } else {
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), nullptr);
  }
}

auto pMenu::setText(const string& text) -> void {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), _mnemonic(text));
}

}

#endif
