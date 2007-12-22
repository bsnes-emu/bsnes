void pMenuControl::enable(bool state) {
  EnableMenuItem(parent, instance, MF_BYCOMMAND | (state ? MF_ENABLED : MF_GRAYED));
}

void pMenuControl::disable() {
  enable(false);
}

bool pMenuControl::enabled() {
MENUITEMINFO info;
  memset(&info, 0, sizeof info);
  info.cbSize = sizeof info;
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, instance, false, &info);
  return info.fState & MFS_ENABLED;
}

pMenuControl::pMenuControl(MenuControl &self_) : pWidget(self_), self(self_) {
  parent = 0;
  text = 0;
}

pMenuControl::~pMenuControl() {
  safe_free(text);
}
