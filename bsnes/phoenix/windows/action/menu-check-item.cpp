void MenuCheckItem::setText(const string &text) {
  action->text = text;
}

bool MenuCheckItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuCheckItem::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

bool MenuCheckItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return info.fState & MFS_CHECKED;
}

void MenuCheckItem::setChecked(bool checked) {
  action->checked = checked;
  if(action->parent) CheckMenuItem(action->parent->action->menu, object->id, checked ? MF_CHECKED : MF_UNCHECKED);
}
