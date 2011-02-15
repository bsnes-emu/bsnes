void pButton::setText(const string &text) {
  qtButton->setText(QString::fromUtf8(text));
}

pButton::pButton(Button &button) : button(button), pWidget(button) {
  qtWidget = qtButton = new QPushButton;
  connect(qtButton, SIGNAL(released()), SLOT(onTick()));
}

void pButton::onTick() {
  if(button.onTick) button.onTick();
}
