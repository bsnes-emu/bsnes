Action::Action() {
  OS::os->objects.append(this);
  action = new Action::Data;
}

void Menu::create(Window &parent, const char *text) {
  action->parentMenu = parent.window->menu;
  action->menu = CreatePopupMenu();
  AppendMenu(parent.window->menu, MF_STRING | MF_POPUP, (UINT_PTR)action->menu, utf16_t(text));
}

void Menu::create(Menu &parent, const char *text) {
  action->parentMenu = parent.action->menu;
  action->menu = CreatePopupMenu();
  AppendMenu(parent.action->menu, MF_STRING | MF_POPUP, (UINT_PTR)action->menu, utf16_t(text));
}

bool Menu::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parentMenu, (UINT_PTR)action->menu, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void Menu::setEnabled(bool enabled) {
  EnableMenuItem(action->parentMenu, (UINT_PTR)action->menu, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void MenuSeparator::create(Menu &parent) {
  action->parent = &parent;
  AppendMenu(parent.action->menu, MF_SEPARATOR, object->id, L"");
}

bool MenuSeparator::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuSeparator::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void MenuItem::create(Menu &parent, const char *text) {
  action->parent = &parent;
  AppendMenu(parent.action->menu, MF_STRING, object->id, utf16_t(text));
}

bool MenuItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuItem::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void MenuCheckItem::create(Menu &parent, const char *text) {
  action->parent = &parent;
  AppendMenu(parent.action->menu, MF_STRING, object->id, utf16_t(text));
}

bool MenuCheckItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuCheckItem::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

bool MenuCheckItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return info.fState & MFS_CHECKED;
}

void MenuCheckItem::setChecked(bool checked) {
  CheckMenuItem(action->parent->action->menu, object->id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void MenuRadioItem::create(Menu &parent, const char *text) {
  action->parent = &parent;
  action->radioParent = this;
  action->items.append(this);
  AppendMenu(parent.action->menu, MF_STRING, object->id, utf16_t(text));
  setChecked();
}

void MenuRadioItem::create(MenuRadioItem &parent, const char *text) {
  action->parent = parent.action->parent;
  action->radioParent = parent.action->radioParent;
  action->radioParent->action->items.append(this);
  AppendMenu(action->parent->action->menu, MF_STRING, object->id, utf16_t(text));
}

bool MenuRadioItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuRadioItem::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

bool MenuRadioItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return info.fState & MFS_CHECKED;
}

void MenuRadioItem::setChecked() {
  MenuRadioItem *parent = action->radioParent;
  foreach(item, parent->action->items) {
    CheckMenuRadioItem(
      action->parent->action->menu,
      item->object->id, item->object->id, item->object->id + (item != this),
      MF_BYCOMMAND
    );
  }
}
