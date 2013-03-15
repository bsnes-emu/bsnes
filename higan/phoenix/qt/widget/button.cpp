namespace phoenix {

Size pButton::minimumSize() {
  Size size = pFont::size(qtWidget->font(), button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    size.width += button.state.image.width;
    size.height = max(button.state.image.height, size.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    size.width = max(button.state.image.width, size.width);
    size.height += button.state.image.height;
  }

  return {size.width + 20, size.height + 12};
}

void pButton::setImage(const image &image, Orientation orientation) {
  qtButton->setIconSize(QSize(image.width, image.height));
  qtButton->setIcon(CreateIcon(image));
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

}
