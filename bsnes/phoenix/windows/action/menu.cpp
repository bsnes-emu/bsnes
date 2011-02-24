void pMenu::append(Action &action) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenu::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenu::constructor() {
  hmenu = 0;
}

//Windows actions lack the ability to toggle visibility.
//To support this, menus must be destroyed and recreated when toggling any action's visibility.
void pMenu::update(Window &parentWindow, HMENU parentMenu) {
  this->parentWindow = &parentWindow;
  this->parentMenu = parentMenu;
  if(hmenu) DestroyMenu(hmenu);
  hmenu = CreatePopupMenu();

  foreach(action, menu.state.action) {
    action.p.parentWindow = &parentWindow;
    action.p.parentMenu = hmenu;

    unsigned enabled = action.state.enabled ? 0 : MF_GRAYED;
    if(dynamic_cast<Menu*>(&action)) {
      Menu &item = (Menu&)action;
      item.p.update(parentWindow, hmenu);
      AppendMenu(hmenu, MF_STRING | MF_POPUP | enabled, (UINT_PTR)item.p.hmenu, utf16_t(item.state.text));
    } else if(dynamic_cast<MenuSeparator*>(&action)) {
      MenuSeparator &item = (MenuSeparator&)action;
      if(action.state.visible) AppendMenu(hmenu, MF_SEPARATOR | enabled, item.p.id, L"");
    } else if(dynamic_cast<MenuItem*>(&action)) {
      MenuItem &item = (MenuItem&)action;
      if(action.state.visible) AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));
    } else if(dynamic_cast<MenuCheckItem*>(&action)) {
      MenuCheckItem &item = (MenuCheckItem&)action;
      if(action.state.visible) AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));
      if(item.state.checked) item.setChecked();
    } else if(dynamic_cast<MenuRadioItem*>(&action)) {
      MenuRadioItem &item = (MenuRadioItem&)action;
      if(action.state.visible) AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));
      if(item.state.checked) item.setChecked();
    }
  }
}
