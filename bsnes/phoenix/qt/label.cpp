void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  label->setParent(parent.window->container);
  label->setGeometry(x, y, width, height);
  label->setText(text);
  if(parent.window->defaultFont) label->setFont(*parent.window->defaultFont);
  label->show();
}

Label::Label() {
  label = new Label::Data(*this);
  widget->widget = label;
}
