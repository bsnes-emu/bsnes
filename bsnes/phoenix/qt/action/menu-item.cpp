void MenuItem::setText(const string &text) {
  menuItem->setText(QString::fromUtf8(text));
}

bool MenuItem::visible() {
  return menuItem->isVisible();
}

void MenuItem::setVisible(bool visible) {
  menuItem->setVisible(visible);
}

bool MenuItem::enabled() {
  return menuItem->isEnabled();
}

void MenuItem::setEnabled(bool enabled) {
  menuItem->setEnabled(enabled);
}

MenuItem::MenuItem() {
  menuItem = new MenuItem::Data(*this);
  menuItem->connect(menuItem, SIGNAL(triggered()), SLOT(onTick()));
}
