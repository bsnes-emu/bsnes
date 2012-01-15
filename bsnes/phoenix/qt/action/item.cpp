void pItem::setImage(const image &image) {
  nall::image qtBuffer = image;
  qtBuffer.transform(0, 32u, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  QImage qtImage(qtBuffer.data, qtBuffer.width, qtBuffer.height, QImage::Format_ARGB32);
  QIcon qtIcon(QPixmap::fromImage(qtImage));
  qtAction->setIcon(qtIcon);
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
