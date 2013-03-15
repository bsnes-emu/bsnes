namespace phoenix {

bool pRadioItem::checked() {
  return qtAction->isChecked();
}

void pRadioItem::setChecked() {
  locked = true;
  for(auto &item : radioItem.state.group) {
    bool checkState = item.p.qtAction == qtAction;
    item.state.checked = checkState;
    item.p.qtAction->setChecked(checkState);
  }
  locked = false;
}

void pRadioItem::setGroup(const set<RadioItem&> &group) {
}

void pRadioItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pRadioItem::constructor() {
  qtAction = new QAction(0);
  qtGroup = new QActionGroup(0);
  qtAction->setCheckable(true);
  qtAction->setActionGroup(qtGroup);
  qtAction->setChecked(true);
  connect(qtAction, SIGNAL(triggered()), SLOT(onActivate()));
}

void pRadioItem::destructor() {
  if(action.state.menu) action.state.menu->remove(radioItem);
  delete qtAction;
}

void pRadioItem::onActivate() {
  if(radioItem.state.checked == false) {
    setChecked();
    if(locked == false && radioItem.onActivate) radioItem.onActivate();
  }
}

}
