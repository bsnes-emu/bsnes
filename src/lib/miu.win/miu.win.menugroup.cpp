void pMenuGroup::create(const char *text_) {
  group = CreatePopupMenu();
  text = strdup(text_);
}

void pMenuGroup::attach(MenuControl &menucontrol) {
  switch(menucontrol.type) {
    case Widget::MenuGroupType: {
      AppendMenu(group, MF_STRING | MF_POPUP, (uint)((MenuGroup&)menucontrol).p.group, menucontrol.p.text);
    } break;

    case Widget::MenuItemType:
    case Widget::MenuCheckItemType:
    case Widget::MenuRadioItemType: {
      AppendMenu(group, MF_STRING, menucontrol.p.instance, menucontrol.p.text);
      if(menucontrol.type == Widget::MenuRadioItemType && ((MenuRadioItem&)menucontrol).p.create_checked) {
        CheckMenuItem(group, menucontrol.p.instance, MF_CHECKED);
      }
    } break;

    case Widget::MenuSeparatorType: {
      AppendMenu(group, MF_SEPARATOR, menucontrol.p.instance, "");
    } break;
  }

  menucontrol.p.parent = group;
}

pMenuGroup::pMenuGroup(MenuGroup &self_) : pMenuControl(self_), self(self_) {
  group = 0;
}
