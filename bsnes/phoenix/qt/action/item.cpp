void pItem::setText(const string &text) {
  qtAction->setText(QString::fromUtf8(text));
}

void pItem::constructor() {
  qtAction = new QAction(0);
  connect(qtAction, SIGNAL(triggered()), SLOT(onTick()));
}

void pItem::onTick() {
  if(item.onTick) item.onTick();
}
