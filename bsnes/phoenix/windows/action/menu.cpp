void Menu::setText(const string &text) {
  action->text = text;
}

void Menu::append(Action &item) {
  menu->children.append(&item);
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

Menu::Menu() {
  menu = new Menu::Data;
}

//internal

void Menu::create() {
  action->menu = CreatePopupMenu();
  foreach(child, menu->children) {
    child->action->parent = this;
    if(dynamic_cast<Menu*>(child)) {
      Menu &item = *(Menu*)child;
      item.action->parentMenu = action->menu;
      item.create();
      AppendMenu(action->menu, MF_STRING | MF_POPUP, (UINT_PTR)item.action->menu, utf16_t(item.action->text));
    } else if(dynamic_cast<MenuSeparator*>(child)) {
      AppendMenu(action->menu, MF_SEPARATOR, child->object->id, L"");
    } else if(dynamic_cast<MenuItem*>(child)) {
      AppendMenu(action->menu, MF_STRING, child->object->id, utf16_t(child->action->text));
    } else if(dynamic_cast<MenuCheckItem*>(child)) {
      AppendMenu(action->menu, MF_STRING, child->object->id, utf16_t(child->action->text));
      MenuCheckItem &item = *(MenuCheckItem*)child;
      if(item.action->checked) item.setChecked();
    } else if(dynamic_cast<MenuRadioItem*>(child)) {
      AppendMenu(action->menu, MF_STRING, child->object->id, utf16_t(child->action->text));
      MenuRadioItem &item = *(MenuRadioItem*)child;
      if(item.action->checked) item.setChecked();
    }
  }
}
