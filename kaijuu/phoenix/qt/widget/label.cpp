Geometry pLabel::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), label.state.text);
  return { 0, 0, geometry.width, geometry.height };
}

void pLabel::setText(const string &text) {
  qtLabel->setText(QString::fromUtf8(text));
}

void pLabel::constructor() {
  qtWidget = qtLabel = new QLabel;

  pWidget::synchronizeState();
  setText(label.state.text);
}

void pLabel::destructor() {
  delete qtLabel;
  qtWidget = qtLabel = 0;
}

void pLabel::orphan() {
  destructor();
  constructor();
}
