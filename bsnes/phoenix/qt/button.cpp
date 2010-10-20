void Button::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  button->setParent(parent.window->container);
  button->setGeometry(x, y, width, height);
  button->setText(text);
  if(parent.window->defaultFont) button->setFont(*parent.window->defaultFont);
  button->show();
  button->connect(button, SIGNAL(released()), SLOT(onTick()));
}

Button::Button() {
  button = new Button::Data(*this);
  widget->widget = button;
}
