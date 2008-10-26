void pMenuGroup::create(const char *text_) {
  group = CreatePopupMenu();
  text = strdup(text_);
}

void pMenuGroup::attach(MenuControl &menucontrol) {
  switch(menucontrol.type) {
    case Widget::MenuGroupType: {
      AppendMenu(group, MF_STRING | MF_POPUP, (unsigned)((MenuGroup&)menucontrol).p.group, utf16(menucontrol.p.text));
    } break;

    case Widget::MenuItemType:
    case Widget::MenuCheckItemType:
    case Widget::MenuRadioItemType: {
      AppendMenu(group, MF_STRING, menucontrol.p.instance, utf16(menucontrol.p.text));
      if(menucontrol.type == Widget::MenuRadioItemType && ((MenuRadioItem&)menucontrol).p.create_checked) {
        CheckMenuItem(group, menucontrol.p.instance, MF_CHECKED);
      }
    } break;

    case Widget::MenuSeparatorType: {
      AppendMenu(group, MF_SEPARATOR, menucontrol.p.instance, L"");
    } break;
  }

  menucontrol.p.parent = group;
}

void pMenuGroup::enable(bool state) {
  EnableMenuItem(parent, (unsigned)group, MF_BYCOMMAND | (state ? MF_ENABLED : MF_GRAYED));
}

void pMenuGroup::disable() {
  enable(false);
}

bool pMenuGroup::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, (unsigned)group, false, &info);
  return info.fState & MFS_ENABLED;
}

pMenuGroup::pMenuGroup(MenuGroup &self_) : pMenuControl(self_), self(self_) {
  group = 0;
}
