void Menu::append(Action &action) {
  gtk_menu_shell_append(GTK_MENU_SHELL(object->menu), action.object->widget);
  gtk_widget_show(action.object->widget);
}

void Menu::setText(const string &text) {
  gtk_menu_item_set_label(GTK_MENU_ITEM(object->widget), text);
}

Menu::Menu() {
  object->menu = gtk_menu_new();
  object->widget = gtk_menu_item_new_with_label("");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(object->widget), object->menu);
}
