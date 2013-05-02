namespace phoenix {

void pMenu::append(Action& action) {
  action.state.window = this->action.state.window;

  gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), action.p.widget);
  if(action.state.window && action.state.window->state.menuFont != "") {
    action.p.setFont(action.state.window->state.menuFont);
  }
  gtk_widget_show(action.p.widget);
}

void pMenu::remove(Action& action) {
  action.p.orphan();
  action.state.window = nullptr;
}

void pMenu::setImage(const image& image) {
  if(image.empty() == false) {
    GtkImage* gtkImage = CreateImage(image, true);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), (GtkWidget*)gtkImage);
  } else {
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), nullptr);
  }
}

void pMenu::setText(const string& text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), mnemonic(text));
}

void pMenu::constructor() {
  gtkMenu = gtk_menu_new();
  widget = gtk_image_menu_item_new_with_mnemonic("");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget), gtkMenu);
  setText(menu.state.text);
}

void pMenu::destructor() {
  gtk_widget_destroy(gtkMenu);
  gtk_widget_destroy(widget);
}

void pMenu::orphan() {
  for(auto& action : menu.state.action) action.p.orphan();
  destructor();
  constructor();
  for(auto& action : menu.state.action) append(action);
}

void pMenu::setFont(const string& font) {
  pAction::setFont(font);
  for(auto& item : menu.state.action) item.p.setFont(font);
}

}
