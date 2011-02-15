void pMenuItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

pMenuItem::pMenuItem(MenuItem &menuItem) : menuItem(menuItem), pAction(menuItem) {
  qtAction = new QAction(0);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pMenuItem::onTick() {
  if(menuItem.onTick) menuItem.onTick();
}
