void pMenuCheckItem::create(const char *text_) {
  text = strdup(text_);
}

void pMenuCheckItem::check(bool state) {
bool prev = checked();
  CheckMenuItem(parent, instance, state ? MF_CHECKED : MF_UNCHECKED);
  if(prev != state) {
    if(self.on_tick) self.on_tick(Event(Event::Tick, state, &self));
  }
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
