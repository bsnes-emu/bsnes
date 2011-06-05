bool pWidget::enabled() {
  return false;
}

Font& pWidget::font() {
  throw;
}

Geometry pWidget::minimumGeometry() {
  return { 0, 0, 0, 0 };
}

void pWidget::setEnabled(bool enabled) {
}

void pWidget::setFocused() {
}

void pWidget::setFont(Font &font) {
}

void pWidget::setGeometry(const Geometry &geometry) {
}

void pWidget::setVisible(bool visible) {
}

void pWidget::constructor() {
}
