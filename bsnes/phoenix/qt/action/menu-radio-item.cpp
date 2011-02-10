void MenuRadioItem::setParent(MenuRadioItem &parent) {
  delete menuRadioItem->actionGroup();
  menuRadioItem->setActionGroup(parent.menuRadioItem->actionGroup());
}

void MenuRadioItem::setText(const string &text) {
  menuRadioItem->setText(QString::fromUtf8(text));
}

bool MenuRadioItem::visible() {
  return menuRadioItem->isVisible();
}

void MenuRadioItem::setVisible(bool visible) {
  menuRadioItem->setVisible(visible);
}

bool MenuRadioItem::enabled() {
  return menuRadioItem->isEnabled();
}

void MenuRadioItem::setEnabled(bool enabled) {
  menuRadioItem->setEnabled(enabled);
}

bool MenuRadioItem::checked() {
  return menuRadioItem->isChecked();
}

void MenuRadioItem::setChecked() {
  object->locked = true;
  menuRadioItem->setChecked(true);
  object->locked = false;
}

MenuRadioItem::MenuRadioItem() {
  menuRadioItem = new MenuRadioItem::Data(*this);
  menuRadioItem->setCheckable(true);
  menuRadioItem->setActionGroup(new QActionGroup(0));
  menuRadioItem->connect(menuRadioItem, SIGNAL(changed()), SLOT(onTick()));
}
