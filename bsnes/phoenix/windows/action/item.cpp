void pItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pItem::constructor() {
}

void pItem::destructor() {
  if(parentMenu) parentMenu->remove(item);
}
