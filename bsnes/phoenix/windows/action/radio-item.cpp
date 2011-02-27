bool pRadioItem::checked() {
  return radioItem.state.checked;
}

void pRadioItem::setChecked() {
  foreach(item, radioItem.state.group) {
    //CheckMenuRadioItem takes: lo, hi, id; checking only id when lo <= id <= hi
    //phoenix does not force IDs to be linear, so to uncheck id, we use: lo == hi == id + 1 (out of range)
    //to check id, we use: lo == hi == id (only ID, but in range)
    if(item.p.parentMenu) CheckMenuRadioItem(item.p.parentMenu, item.p.id, item.p.id, item.p.id + (id != item.p.id), MF_BYCOMMAND);
  }
}

void pRadioItem::setGroup(const reference_array<RadioItem&> &group) {
}

void pRadioItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pRadioItem::constructor() {
}
