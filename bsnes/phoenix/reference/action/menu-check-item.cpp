bool pMenuCheckItem::checked() {
  return false;
}

void pMenuCheckItem::setChecked(bool checked) {
}

void pMenuCheckItem::setText(const string &text) {
}

pMenuCheckItem::pMenuCheckItem(MenuCheckItem &menuCheckItem) : pAction(menuCheckItem), menuCheckItem(menuCheckItem) {
}
