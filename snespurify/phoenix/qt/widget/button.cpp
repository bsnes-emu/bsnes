void pButton::setText(const string &text) {
  qtButton->setText(QString::fromUtf8(text));
}

void pButton::constructor() {
  qtWidget = qtButton = new QPushButton;
  connect(qtButton, SIGNAL(released()), SLOT(onTick()));
}

void pButton::onTick() {
  if(button.onTick) button.onTick();
}
