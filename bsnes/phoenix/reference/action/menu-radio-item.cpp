bool pMenuRadioItem::checked() {
  return false;
}

void pMenuRadioItem::setChecked() {
}

void pMenuRadioItem::setGroup(const array<MenuRadioItem*> &group) {
}

void pMenuRadioItem::setText(const string &text) {
}

pMenuRadioItem::pMenuRadioItem(MenuRadioItem &menuRadioItem) : pAction(menuRadioItem), menuRadioItem(menuRadioItem) {
}
