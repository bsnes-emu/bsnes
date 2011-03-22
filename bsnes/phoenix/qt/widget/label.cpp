Geometry pLabel::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(label.state.text);
  return { 0, 0, geometry.width, geometry.height };
}

void pLabel::setText(const string &text) {
  qtLabel->setText(QString::fromUtf8(text));
}

void pLabel::constructor() {
  qtWidget = qtLabel = new QLabel;
}
