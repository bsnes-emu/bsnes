namespace phoenix {

Size pCheckButton::minimumSize() {
  Size size = pFont::size(qtWidget->font(), checkButton.state.text);

  if(checkButton.state.orientation == Orientation::Horizontal) {
    size.width += checkButton.state.image.width;
    size.height = max(checkButton.state.image.height, size.height);
  }

  if(checkButton.state.orientation == Orientation::Vertical) {
    size.width = max(checkButton.state.image.width, size.width);
    size.height += checkButton.state.image.height;
  }

  return {size.width + 20, size.height + 12};
}

void pCheckButton::setChecked(bool checked) {
  locked = true;
  qtCheckButton->setChecked(checked);
  locked = false;
}

void pCheckButton::setImage(const image& image, Orientation orientation) {
  qtCheckButton->setIconSize(QSize(image.width, image.height));
  qtCheckButton->setIcon(CreateIcon(image));
  qtCheckButton->setStyleSheet("text-align: top;");
  switch(orientation) {
  case Orientation::Horizontal: qtCheckButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); break;
  case Orientation::Vertical:   qtCheckButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); break;
  }
}

void pCheckButton::setText(string text) {
  qtCheckButton->setText(QString::fromUtf8(text));
}

void pCheckButton::constructor() {
  qtWidget = qtCheckButton = new QToolButton;
  qtCheckButton->setCheckable(true);
  qtCheckButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  connect(qtCheckButton, SIGNAL(toggled(bool)), SLOT(onToggle(bool)));

  pWidget::synchronizeState();
  setChecked(checkButton.state.checked);
  setText(checkButton.state.text);
}

void pCheckButton::destructor() {
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

void pCheckButton::onToggle(bool checked) {
  checkButton.state.checked = checked;
  if(!locked && checkButton.onToggle) checkButton.onToggle();
}

}
