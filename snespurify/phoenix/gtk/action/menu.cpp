void pMenu::append(Action &action) {
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), action.p.widget);
  gtk_widget_show(action.p.widget);
}

void pMenu::setFont(Font &font) {
  pAction::setFont(font);
  foreach(item, menu.state.action) item.p.setFont(font);
}

void pMenu::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), text);
}

void pMenu::constructor() {
  submenu = gtk_menu_new();
  widget = gtk_menu_item_new_with_label("");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget), submenu);
}
