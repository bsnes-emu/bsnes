void pMenuCheckItem::create(const char *text_) {
  text = strdup(text_);
}

void pMenuCheckItem::check(bool state) {
  CheckMenuItem(parent, instance, state ? MF_CHECKED : MF_UNCHECKED);
}

void pMenuCheckItem::uncheck() {
  check(false);
}

bool pMenuCheckItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_CHECKED;
}

pMenuCheckItem::pMenuCheckItem(MenuCheckItem &self_) : pMenuControl(self_), self(self_) {
}
