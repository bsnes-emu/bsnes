void pLabel::setText(const string &text) {
  qtLabel->setText(QString::fromUtf8(text));
}

void pLabel::constructor() {
  qtWidget = qtLabel = new QLabel;
}
