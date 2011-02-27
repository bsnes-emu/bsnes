bool pCheckItem::checked() {
  return checkItem.state.checked;
}

void pCheckItem::setChecked(bool checked) {
  if(parentMenu) CheckMenuItem(parentMenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void pCheckItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pCheckItem::constructor() {
}
