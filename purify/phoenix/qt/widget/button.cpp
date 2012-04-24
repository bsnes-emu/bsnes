Geometry pButton::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    geometry.width += button.state.image.width;
    geometry.height = max(button.state.image.height, geometry.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    geometry.width = max(button.state.image.width, geometry.width);
    geometry.height += button.state.image.height;
  }

  return { 0, 0, geometry.width + 20, geometry.height + 12 };
}

void pButton::setImage(const image &image, Orientation orientation) {
  nall::image qtBuffer = image;
  qtBuffer.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);

  QImage qtImage(qtBuffer.data, qtBuffer.width, qtBuffer.height, QImage::Format_ARGB32);
  QIcon qtIcon(QPixmap::fromImage(qtImage));
  qtButton->setIconSize(QSize(qtBuffer.width, qtBuffer.height));
  qtButton->setIcon(qtIcon);
  qtButton->setStyleSheet("text-align: top;");
  switch(orientation) {
  case Orientation::Horizontal: qtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); break;
  case Orientation::Vertical:   qtButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  break;
  }
}

void pButton::setText(const string &text) {
  qtButton->setText(QString::fromUtf8(text));
}

void pButton::constructor() {
  qtWidget = qtButton = new QToolButton;
  qtButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  connect(qtButton, SIGNAL(released()), SLOT(onActivate()));

  pWidget::synchronizeState();
  setText(button.state.text);
}

void pButton::destructor() {
  delete qtButton;
  qtWidget = qtButton = 0;
}

void pButton::orphan() {
  destructor();
  constructor();
}

void pButton::onActivate() {
  if(button.onActivate) button.onActivate();
}
