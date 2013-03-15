namespace phoenix {

void pItem::setImage(const image &image) {
  qtAction->setIcon(CreateIcon(image));
}

void pItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pItem::constructor() {
  qtAction = new QAction(0);
  connect(qtAction, SIGNAL(triggered()), SLOT(onActivate()));
}

void pItem::destructor() {
  if(action.state.menu) action.state.menu->remove(item);
  delete qtAction;
}

void pItem::onActivate() {
  if(item.onActivate) item.onActivate();
}

}
