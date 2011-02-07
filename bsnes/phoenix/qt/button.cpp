void Button::setText(const string &text) {
  button->setText(QString::fromUtf8(text));
}

Button::Button() {
  button = new Button::Data(*this);
  widget->widget = button;
  button->connect(button, SIGNAL(released()), SLOT(onTick()));
}
