void pMenuItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pMenuItem::constructor() {
  qtAction = new QAction(0);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pMenuItem::onTick() {
  if(menuItem.onTick) menuItem.onTick();
}
