namespace phoenix {

Size pLabel::minimumSize() {
  Size size = pFont::size(qtWidget->font(), label.state.text);
  return {size.width, size.height};
}

void pLabel::setText(const string& text) {
  qtLabel->setText(QString::fromUtf8(text));
}

void pLabel::constructor() {
  qtWidget = qtLabel = new QLabel;

  pWidget::synchronizeState();
  setText(label.state.text);
}

void pLabel::destructor() {
  delete qtLabel;
  qtWidget = qtLabel = nullptr;
}

void pLabel::orphan() {
  destructor();
  constructor();
}

}
