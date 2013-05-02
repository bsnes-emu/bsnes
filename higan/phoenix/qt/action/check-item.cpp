namespace phoenix {

bool pCheckItem::checked() {
  return qtAction->isChecked();
}

void pCheckItem::setChecked(bool checked) {
  qtAction->setChecked(checked);
}

void pCheckItem::setText(const string& text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pCheckItem::constructor() {
  qtAction = new QAction(0);
  qtAction->setCheckable(true);
  connect(qtAction, SIGNAL(triggered()), SLOT(onToggle()));
}

void pCheckItem::destructor() {
  if(action.state.menu) action.state.menu->remove(checkItem);
  delete qtAction;
  qtAction = nullptr;
}

void pCheckItem::onToggle() {
  checkItem.state.checked = checked();
  if(checkItem.onToggle) checkItem.onToggle();
}

}
