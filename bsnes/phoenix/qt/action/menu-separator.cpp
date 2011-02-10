bool MenuSeparator::visible() {
  return menuSeparator->action->isVisible();
}

void MenuSeparator::setVisible(bool visible) {
  menuSeparator->action->setVisible(visible);
}

bool MenuSeparator::enabled() {
  return menuSeparator->action->isEnabled();
}

void MenuSeparator::setEnabled(bool enabled) {
  menuSeparator->action->setEnabled(enabled);
}

MenuSeparator::MenuSeparator() {
  menuSeparator = new MenuSeparator::Data(*this);
}
