bool pCheckItem::checked() {
  return qtAction->isChecked();
}

void pCheckItem::setChecked(bool checked) {
  qtAction->setChecked(checked);
}

void pCheckItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pCheckItem::constructor() {
  qtAction = new QAction(0);
  qtAction->setCheckable(true);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pCheckItem::destructor() {
  if(action.state.menu) action.state.menu->remove(checkItem);
  delete qtAction;
}

void pCheckItem::onTick() {
  checkItem.state.checked = checked();
  if(checkItem.onTick) checkItem.onTick();
}
