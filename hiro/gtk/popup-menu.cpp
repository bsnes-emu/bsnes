namespace hiro {

auto pPopupMenu::construct() -> void {
  gtkMenu = gtk_menu_new();
}

auto pPopupMenu::destruct() -> void {
  gtk_widget_destroy(gtkMenu);
}

auto pPopupMenu::append(sAction action) -> void {
  if(action->self()) {
    gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), action->self()->widget);
    action->self()->setFont(action->font(true));
    action->self()->setVisible(action->visible(true));
  }
}

auto pPopupMenu::remove(sAction action) -> void {
}

auto pPopupMenu::setFont(const string& font) -> void {
  for(auto& action : state().actions) {
    if(action->self()) action->self()->setFont(action->font(true));
  }
}

auto pPopupMenu::setVisible(bool visible) -> void {
  if(visible) gtk_menu_popup(GTK_MENU(gtkMenu), nullptr, nullptr, nullptr, nullptr, 0, gtk_get_current_event_time());
}

}
