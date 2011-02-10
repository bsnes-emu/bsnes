void MenuCheckItem::setText(const string &text) {
  menuCheckItem->setText(QString::fromUtf8(text));
}

bool MenuCheckItem::visible() {
  return menuCheckItem->isVisible();
}

void MenuCheckItem::setVisible(bool visible) {
  menuCheckItem->setVisible(visible);
}

bool MenuCheckItem::enabled() {
  return menuCheckItem->isEnabled();
}

void MenuCheckItem::setEnabled(bool enabled) {
  menuCheckItem->setEnabled(enabled);
}

bool MenuCheckItem::checked() {
  return menuCheckItem->isChecked();
}

void MenuCheckItem::setChecked(bool checked) {
  menuCheckItem->setChecked(checked);
}

MenuCheckItem::MenuCheckItem() {
  menuCheckItem = new MenuCheckItem::Data(*this);
  menuCheckItem->setCheckable(true);
  menuCheckItem->connect(menuCheckItem, SIGNAL(triggered()), SLOT(onTick()));
}
