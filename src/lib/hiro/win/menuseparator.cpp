void pMenuSeparator::create() {
}

void pMenuSeparator::enable(bool state) {
  EnableMenuItem(parent, instance, MF_BYCOMMAND | (state ? MF_ENABLED : MF_GRAYED));
}

void pMenuSeparator::disable() {
  enable(false);
}

bool pMenuSeparator::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_ENABLED;
}

pMenuSeparator::pMenuSeparator(MenuSeparator &self_) : pMenuControl(self_), self(self_) {
}
