namespace phoenix {

void pMenu::append(Action& action) {
  action.p.parentMenu = &menu;
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenu::remove(Action& action) {
  if(parentWindow) parentWindow->p.updateMenu();
  action.p.parentMenu = 0;
}

void pMenu::setImage(const image& image) {
  createBitmap();
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenu::setText(string text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenu::constructor() {
  hmenu = 0;
  createBitmap();
}

void pMenu::destructor() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }
  if(parentMenu) {
    parentMenu->remove(menu);
  } else if(parentWindow) {
    //belongs to window's main menubar
    parentWindow->remove(menu);
  }
}

void pMenu::createBitmap() {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }

  if(menu.state.image.width && menu.state.image.height) {
    nall::image nallImage = menu.state.image;
    nallImage.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    nallImage.alphaBlend(GetSysColor(COLOR_MENU));  //Windows does not alpha blend menu icons properly (leaves black outline)
    nallImage.scale(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK), Interpolation::Linear);
    hbitmap = CreateBitmap(nallImage);
  }
}

//Windows actions lack the ability to toggle visibility.
//To support this, menus must be destroyed and recreated when toggling any action's visibility.
void pMenu::update(Window& parentWindow, Menu* parentMenu) {
  this->parentMenu = parentMenu;
  this->parentWindow = &parentWindow;

  if(hmenu) DestroyMenu(hmenu);
  hmenu = CreatePopupMenu();

  for(auto& action : menu.state.action) {
    action.p.parentMenu = &menu;
    action.p.parentWindow = &parentWindow;

    unsigned enabled = action.state.enabled ? 0 : MF_GRAYED;
    if(dynamic_cast<Menu*>(&action)) {
      Menu& item = (Menu&)action;
      if(action.state.visible) {
        item.p.update(parentWindow, &menu);
        AppendMenu(hmenu, MF_STRING | MF_POPUP | enabled, (UINT_PTR)item.p.hmenu, utf16_t(item.state.text));

        if(item.state.image.width && item.state.image.height) {
          MENUITEMINFO mii = {sizeof(MENUITEMINFO)};
          //Windows XP and below displays MIIM_BITMAP + hbmpItem in its own column (separate from check/radio marks)
          //this causes too much spacing, so use a custom checkmark image instead
          mii.fMask = MIIM_CHECKMARKS;
          mii.hbmpUnchecked = item.p.hbitmap;
          SetMenuItemInfo(hmenu, (UINT_PTR)item.p.hmenu, FALSE, &mii);
        }
      }
    } else if(dynamic_cast<Separator*>(&action)) {
      Separator& item = (Separator&)action;
      if(action.state.visible) {
        AppendMenu(hmenu, MF_SEPARATOR | enabled, item.p.id, L"");
      }
    } else if(dynamic_cast<Item*>(&action)) {
      Item& item = (Item&)action;
      if(action.state.visible) {
        AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));

        if(item.state.image.width && item.state.image.height) {
          MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
          //Windows XP and below displays MIIM_BITMAP + hbmpItem in its own column (separate from check/radio marks)
          //this causes too much spacing, so use a custom checkmark image instead
          mii.fMask = MIIM_CHECKMARKS;
          mii.hbmpUnchecked = item.p.hbitmap;
          SetMenuItemInfo(hmenu, item.p.id, FALSE, &mii);
        }
      }
    } else if(dynamic_cast<CheckItem*>(&action)) {
      CheckItem& item = (CheckItem&)action;
      if(action.state.visible) {
        AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));
      }
      if(item.state.checked) item.setChecked();
    } else if(dynamic_cast<RadioItem*>(&action)) {
      RadioItem& item = (RadioItem&)action;
      if(action.state.visible) {
        AppendMenu(hmenu, MF_STRING | enabled, item.p.id, utf16_t(item.state.text));
      }
      if(item.state.checked) item.setChecked();
    }
  }
}

}
