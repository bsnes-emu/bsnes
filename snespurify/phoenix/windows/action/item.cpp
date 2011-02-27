void pItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pItem::constructor() {
}
