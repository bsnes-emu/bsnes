static void Action_setFont(GtkWidget *widget, gpointer font) {
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)Action_setFont, font);
  }
}

void Action::setFont(Font &font) {
  Action_setFont(object->widget, font.font->font);
}

bool Action::visible() {
  return gtk_widget_get_visible(object->widget);
}

void Action::setVisible(bool visible) {
  gtk_widget_set_visible(object->widget, visible);
}

bool Action::enabled() {
  return gtk_widget_get_sensitive(object->widget);
}

void Action::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(object->widget, enabled);
}

void Menu::create(Window &parent, const char *text) {
  object->menu = gtk_menu_new();
  object->widget = gtk_menu_item_new_with_label(text);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(object->widget), object->menu);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_menu_bar_append(parent.object->menu, object->widget);
  gtk_widget_show(object->widget);
}

void Menu::create(Menu &parent, const char *text) {
  object->menu = gtk_menu_new();
  object->widget = gtk_menu_item_new_with_label(text);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(object->widget), object->menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(parent.object->menu), object->widget);
  gtk_widget_show(object->widget);
}

void MenuSeparator::create(Menu &parent) {
  object->widget = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(parent.object->menu), object->widget);
  gtk_widget_show(object->widget);
}

static void MenuItem_tick(MenuItem *self) {
  if(self->onTick) self->onTick();
}

void MenuItem::create(Menu &parent, const char *text) {
  object->widget = gtk_menu_item_new_with_label(text);
  g_signal_connect_swapped(G_OBJECT(object->widget), "activate", G_CALLBACK(MenuItem_tick), (gpointer)this);
  gtk_menu_shell_append(GTK_MENU_SHELL(parent.object->menu), object->widget);
  gtk_widget_show(object->widget);
}

static void MenuCheckItem_tick(MenuCheckItem *self) {
  if(self->onTick && self->object->locked == false) self->onTick();
}

void MenuCheckItem::create(Menu &parent, const char *text) {
  object->widget = gtk_check_menu_item_new_with_label(text);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(MenuCheckItem_tick), (gpointer)this);
  gtk_menu_shell_append(GTK_MENU_SHELL(parent.object->menu), object->widget);
  gtk_widget_show(object->widget);
}

bool MenuCheckItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(object->widget));
}

void MenuCheckItem::setChecked(bool state) {
  object->locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(object->widget), state);
  object->locked = false;
}

static void MenuRadioItem_tick(MenuRadioItem *self) {
  if(self->onTick && self->checked() && self->object->locked == false) self->onTick();
}

void MenuRadioItem::create(Menu &parent, const char *text) {
  first = this;
  object->parentMenu = &parent;
  object->widget = gtk_radio_menu_item_new_with_label(0, text);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(MenuRadioItem_tick), (gpointer)this);
  gtk_menu_shell_append(GTK_MENU_SHELL(parent.object->menu), object->widget);
  gtk_widget_show(object->widget);
}

void MenuRadioItem::create(MenuRadioItem &parent, const char *text) {
  first = parent.first;
  object->parentMenu = parent.object->parentMenu;
  object->widget = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(first->object->widget), text);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(MenuRadioItem_tick), (gpointer)this);
  gtk_menu_shell_append(GTK_MENU_SHELL(object->parentMenu->object->menu), object->widget);
  gtk_widget_show(object->widget);
}

bool MenuRadioItem::checked() {
  return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(object->widget));
}

void MenuRadioItem::setChecked() {
  object->locked = true;
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(object->widget), true);
  object->locked = false;
}
