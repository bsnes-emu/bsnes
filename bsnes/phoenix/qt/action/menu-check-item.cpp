bool pMenuCheckItem::checked() {
  return qtAction->isChecked();
}

void pMenuCheckItem::setChecked(bool checked) {
  qtAction->setChecked(checked);
}

void pMenuCheckItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pMenuCheckItem::constructor() {
  qtAction = new QAction(0);
  qtAction->setCheckable(true);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pMenuCheckItem::onTick() {
  menuCheckItem.state.checked = checked();
  if(menuCheckItem.onTick) menuCheckItem.onTick();
}
