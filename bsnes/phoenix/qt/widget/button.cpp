Geometry pButton::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), button.state.text);
  return { 0, 0, geometry.width + 20, geometry.height + 12 };
}

void pButton::setText(const string &text) {
  qtButton->setText(QString::fromUtf8(text));
}

void pButton::constructor() {
  qtWidget = qtButton = new QPushButton;
  connect(qtButton, SIGNAL(released()), SLOT(onTick()));

  pWidget::synchronizeState();
  setText(button.state.text);
}

void pButton::destructor() {
  delete qtButton;
  qtWidget = qtButton = 0;
}

void pButton::orphan() {
  destructor();
  constructor();
}

void pButton::onTick() {
  if(button.onTick) button.onTick();
}
