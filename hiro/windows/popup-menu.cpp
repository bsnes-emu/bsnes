#if defined(Hiro_PopupMenu)

namespace hiro {

auto pPopupMenu::construct() -> void {
  hwnd = CreateWindow(L"hiroPopupMenu", L"", ResizableStyle, 0, 0, 0, 0, 0, 0, GetModuleHandle(0), 0);
}

auto pPopupMenu::destruct() -> void {
  if(hmenu) { DestroyMenu(hmenu); hmenu = nullptr; }
  DestroyWindow(hwnd);
}

auto pPopupMenu::append(sAction action) -> void {
}

auto pPopupMenu::remove(sAction action) -> void {
}

auto pPopupMenu::setFont(const string& font) -> void {
}

auto pPopupMenu::setVisible(bool visible) -> void {
  if(!visible) return;

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

  POINT point{0};
  GetCursorPos(&point);
  TrackPopupMenu(hmenu, TPM_LEFTALIGN | TPM_TOPALIGN, point.x, point.y, 0, hwnd, nullptr);
}

}

#endif
