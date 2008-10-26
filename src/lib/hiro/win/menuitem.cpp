void pMenuItem::create(const char *text_) {
  text = strdup(text_);
}

void pMenuItem::enable(bool state) {
  EnableMenuItem(parent, instance, MF_BYCOMMAND | (state ? MF_ENABLED : MF_GRAYED));
}

void pMenuItem::disable() {
  enable(false);
}

bool pMenuItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_ENABLED;
}

pMenuItem::pMenuItem(MenuItem &self_) : pMenuControl(self_), self(self_) {
}
