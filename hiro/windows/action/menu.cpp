#if defined(Hiro_Menu)

namespace hiro {

auto pMenu::construct() -> void {
  _createBitmap();
}

auto pMenu::destruct() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = nullptr; }
  if(hmenu) { DestroyMenu(hmenu); hmenu = nullptr; }
}

auto pMenu::append(sAction action) -> void {
  _synchronize();
}

auto pMenu::remove(sAction action) -> void {
  _synchronize();
}

auto pMenu::setIcon(const image& icon) -> void {
  _createBitmap();
  _synchronize();
}

auto pMenu::setText(const string& text) -> void {
  _synchronize();
}

auto pMenu::_createBitmap() -> void {
  if(hbitmap) { DeleteObject(hbitmap); hbitmap = 0; }

  if(auto icon = state().icon) {
    icon.transform();
    icon.alphaBlend(GetSysColor(COLOR_MENU));  //Windows does not alpha blend menu icons properly (leaves black outline)
    icon.scale(GetSystemMetrics(SM_CXMENUCHECK), GetSystemMetrics(SM_CYMENUCHECK), Interpolation::Linear);
    hbitmap = CreateBitmap(icon);
  }
}

//Windows actions lack the ability to toggle visibility.
//To support this, menus must be destroyed and recreated when toggling any action's visibility.
auto pMenu::_update() -> void {
  if(hmenu) DestroyMenu(hmenu);
  hmenu = CreatePopupMenu();

  MENUINFO mi{sizeof(MENUINFO)};
  mi.fMask = MIM_STYLE;
  mi.dwStyle = MNS_NOTIFYBYPOS;  //| MNS_MODELESS;
  SetMenuInfo(hmenu, &mi);

  unsigned position = 0;

  for(auto& action : state().actions) {
    if(!action->self()) continue;
    action->self()->position = position;
    unsigned enabled = action->enabled() ? 0 : MF_GRAYED;

    MENUITEMINFO mii{sizeof(MENUITEMINFO)};
    mii.fMask = MIIM_DATA;
    mii.dwItemData = (ULONG_PTR)action.data();

    if(auto menu = dynamic_cast<mMenu*>(action.data())) {
      if(menu->visible()) {
        menu->self()->_update();
        AppendMenu(hmenu, MF_STRING | MF_POPUP | enabled, (UINT_PTR)menu->self()->hmenu, utf16_t(menu->text()));
        if(auto bitmap = menu->self()->hbitmap) {
          //Windows XP and below displays MIIM_BITMAP + hbmpItem in its own column (separate from check/radio marks)
          //this causes too much spacing, so use a custom checkmark image instead
          mii.fMask |= MIIM_CHECKMARKS;
          mii.hbmpUnchecked = bitmap;
        }
        SetMenuItemInfo(hmenu, position++, true, &mii);
      }
    }

    #if defined(Hiro_MenuSeparator)
    else if(auto menuSeparator = dynamic_cast<mMenuSeparator*>(action.data())) {
      if(menuSeparator->visible()) {
        AppendMenu(hmenu, MF_SEPARATOR | enabled, position, L"");
        SetMenuItemInfo(hmenu, position++, true, &mii);
      }
    }
    #endif

    #if defined(Hiro_MenuItem)
    else if(auto menuItem = dynamic_cast<mMenuItem*>(action.data())) {
      if(menuItem->visible()) {
        AppendMenu(hmenu, MF_STRING | enabled, position, utf16_t(menuItem->text()));
        if(auto bitmap = menuItem->self()->hbitmap) {
          mii.fMask |= MIIM_CHECKMARKS;
          mii.hbmpUnchecked = bitmap;
        }
        SetMenuItemInfo(hmenu, position++, true, &mii);
      }
    }
    #endif

    #if defined(Hiro_MenuCheckItem)
    else if(auto menuCheckItem = dynamic_cast<mMenuCheckItem*>(action.data())) {
      if(menuCheckItem->visible()) {
        AppendMenu(hmenu, MF_STRING | enabled, position, utf16_t(menuCheckItem->text()));
        SetMenuItemInfo(hmenu, position++, true, &mii);
        if(menuCheckItem->checked()) menuCheckItem->setChecked();
      }
    }
    #endif

    #if defined(Hiro_MenuRadioItem)
    else if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(action.data())) {
      if(menuRadioItem->visible()) {
        AppendMenu(hmenu, MF_STRING | enabled, position, utf16_t(menuRadioItem->text()));
        SetMenuItemInfo(hmenu, position++, true, &mii);
        if(menuRadioItem->checked()) menuRadioItem->setChecked();
      }
    }
    #endif
  }
}

}

#endif
