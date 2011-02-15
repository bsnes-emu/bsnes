void pLabel::setText(const string &text) {
  qtLabel->setText(QString::fromUtf8(text));
}

pLabel::pLabel(Label &label) : label(label), pWidget(label) {
  qtWidget = qtLabel = new QLabel;
}
