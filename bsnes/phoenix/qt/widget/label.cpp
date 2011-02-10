void Label::setText(const string &text) {
  label->setText(QString::fromUtf8(text));
}

Label::Label() {
  label = new Label::Data(*this);
  widget->widget = label;
}
