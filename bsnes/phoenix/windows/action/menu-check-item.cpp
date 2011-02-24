bool pMenuCheckItem::checked() {
  return menuCheckItem.state.checked;
}

void pMenuCheckItem::setChecked(bool checked) {
  if(parentMenu) CheckMenuItem(parentMenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void pMenuCheckItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenuCheckItem::constructor() {
}
