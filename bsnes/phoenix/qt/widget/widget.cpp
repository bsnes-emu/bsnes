bool pWidget::enabled() {
  return qtWidget->isEnabled();
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
  qtWidget->setVisible(visible);
}

pWidget::pWidget(Widget &widget) : widget(widget) {
}
