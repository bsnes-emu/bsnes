void pMenuGroup::create(const char *text) {
  group = gtk_menu_new();
  item = gtk_menu_item_new_with_label(text ? text : "");
  set_default_font(item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), group);
  gtk_widget_show(item);
}

void pMenuGroup::attach(MenuControl &menucontrol) {
  gtk_menu_shell_append(GTK_MENU_SHELL(group), menucontrol.p.gtk_handle());
}

pMenuGroup::pMenuGroup(MenuGroup &self_) : pMenuControl(self_), self(self_) {
  group = 0;
  item = 0;
}

/* internal */

GtkWidget* pMenuGroup::gtk_handle() {
  return item;
}
