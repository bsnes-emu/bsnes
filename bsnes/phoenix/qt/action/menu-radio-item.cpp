bool pMenuRadioItem::checked() {
  return qtAction->isChecked();
}

void pMenuRadioItem::setChecked() {
  locked = true;
  foreach(item, menuRadioItem.state.group) {
    bool checkState = item.p.qtAction == qtAction;
    item.state.checked = checkState;
    item.p.qtAction->setChecked(checkState);
  }
  locked = false;
}

void pMenuRadioItem::setGroup(const reference_array<MenuRadioItem&> &group) {
}

void pMenuRadioItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pMenuRadioItem::constructor() {
  qtAction = new QAction(0);
  qtGroup = new QActionGroup(0);
  qtAction->setCheckable(true);
  qtAction->setActionGroup(qtGroup);
  qtAction->setChecked(true);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pMenuRadioItem::onTick() {
  if(menuRadioItem.state.checked == false) {
    setChecked();
    if(locked == false && menuRadioItem.onTick) menuRadioItem.onTick();
  }
}
