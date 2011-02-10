bool MenuSeparator::enabled() {
  MENUITEMINFO info;
  memset(&info, 0, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(action->parent->action->menu, object->id, false, &info);
  return (info.fState & MFS_GRAYED) == 0;
}

void MenuSeparator::setEnabled(bool enabled) {
  EnableMenuItem(action->parent->action->menu, object->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}
