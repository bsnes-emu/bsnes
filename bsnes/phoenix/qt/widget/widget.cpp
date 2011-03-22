Font& pWidget::font() {
  if(widget.state.font) return *widget.state.font;
  return pOS::defaultFont;
}

Geometry pWidget::minimumGeometry() {
  return { 0, 0, 100, 25 };
}

void pWidget::setEnabled(bool enabled) {
  qtWidget->setEnabled(enabled);
}

void pWidget::setFocused() {
  qtWidget->setFocus(Qt::OtherFocusReason);
}

void pWidget::setFont(Font &font) {
  qtWidget->setFont(*font.p.qtFont);
}

void pWidget::setGeometry(const Geometry &geometry) {
  qtWidget->setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  qtWidget->setVisible(visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) qtWidget = new QWidget;
}
