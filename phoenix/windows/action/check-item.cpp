namespace phoenix {

void pCheckItem::setChecked(bool checked) {
  if(parentMenu) CheckMenuItem(parentMenu->p.hmenu, id, checked ? MF_CHECKED : MF_UNCHECKED);
}

void pCheckItem::setText(string text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pCheckItem::constructor() {
}

void pCheckItem::destructor() {
  if(parentMenu) parentMenu->remove(checkItem);
}

void pCheckItem::onToggle() {
  checkItem.state.checked = !checkItem.state.checked;
  setChecked(checkItem.state.checked);
  if(checkItem.onToggle) checkItem.onToggle();
}

}
