void MenuRadioItem::setText(const string &text) {
  action->text = text;
}

void MenuRadioItem::setParent(MenuRadioItem &parent) {
  action->checked = false;
  action->radioParent = parent.action->radioParent;
  action->radioParent->action->items.append(this);
}

bool MenuRadioItem::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuRadioItem::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

bool MenuRadioItem::checked() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return info.fState & MFS_CHECKED;
}

void MenuRadioItem::setChecked() {
  action->checked = true;
  MenuRadioItem *parent = action->radioParent;
  foreach(item, parent->action->items) {
    if(action->parent) CheckMenuRadioItem(
      action->parent->action->menu,
      item->object->id, item->object->id, item->object->id + (item != this),
      MF_BYCOMMAND
    );
  }
}

MenuRadioItem::MenuRadioItem() {
  action->radioParent = this;
  action->items.append(this);
  action->checked = true;
}
