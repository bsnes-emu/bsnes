bool pMenuRadioItem::checked() {
  return menuRadioItem.state.checked;
}

void pMenuRadioItem::setChecked() {
  foreach(item, menuRadioItem.state.group) {
    //CheckMenuRadioItem takes: lo, hi, id; checking only id when lo <= id <= hi
    //phoenix does not force IDs to be linear, so to uncheck id, we use: lo == hi == id + 1 (out of range)
    //to check id, we use: lo == hi == id (only ID, but in range)
    if(item.p.parentMenu) CheckMenuRadioItem(item.p.parentMenu, item.p.id, item.p.id, item.p.id + (id != item.p.id), MF_BYCOMMAND);
  }
}

void pMenuRadioItem::setGroup(const reference_array<MenuRadioItem&> &group) {
}

void pMenuRadioItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenuRadioItem::constructor() {
}
