void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  label->setParent(parent.window->container);
  label->setGeometry(x, y, width, height);
  label->setText(QString::fromUtf8(text));
  if(parent.window->defaultFont) label->setFont(*parent.window->defaultFont);
  label->show();
}

void Label::setText(const string &text) {
  label->setText(QString::fromUtf8(text));
}

Label::Label() {
  label = new Label::Data(*this);
  widget->widget = label;
}
