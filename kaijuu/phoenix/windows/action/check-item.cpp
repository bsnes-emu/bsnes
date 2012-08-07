bool pCheckItem::checked() {
  return checkItem.state.checked;
}

void pCheckItem::setChecked(bool checked) {
  if(parentMenu) CheckMenuItem(parentMenu->p.hmenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void pCheckItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pCheckItem::constructor() {
}

void pCheckItem::destructor() {
  if(parentMenu) parentMenu->remove(checkItem);
}
