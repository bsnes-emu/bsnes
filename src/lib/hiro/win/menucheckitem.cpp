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

void pMenuCheckItem::enable(bool state) {
  EnableMenuItem(parent, instance, MF_BYCOMMAND | (state ? MF_ENABLED : MF_GRAYED));
}

void pMenuCheckItem::disable() {
  enable(false);
}

bool pMenuCheckItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_ENABLED;
}

pMenuCheckItem::pMenuCheckItem(MenuCheckItem &self_) : pMenuControl(self_), self(self_) {
}
