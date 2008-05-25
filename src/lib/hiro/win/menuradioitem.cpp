void pMenuRadioItem::create(MenuRadioItemGroup &group_, const char *text_) {
  group = group_;
  text = strdup(text_);
  create_checked = (group[0] == &self);
}

void pMenuRadioItem::check() {
  for(uint i = 0; i < group.size(); i++) {
    CheckMenuItem(parent, group[i]->p.instance, (group[i] == &self) ? MF_CHECKED : MF_UNCHECKED);
  }
}

bool pMenuRadioItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_CHECKED;
}

pMenuRadioItem::pMenuRadioItem(MenuRadioItem &self_) : pMenuControl(self_), self(self_) {
  create_checked = false;
}
