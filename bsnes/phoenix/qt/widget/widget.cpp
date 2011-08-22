Font& pWidget::font() {
  if(widget.state.font) return *widget.state.font;
  return pOS::defaultFont;
}

Geometry pWidget::minimumGeometry() {
  return { 0, 0, 0, 0 };
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

void pWidget::setParent(Window &parent) {
  if(!widget.state.font && parent.state.widgetFont) {
    setFont(*parent.state.widgetFont);
  }
  qtWidget->setParent(parent.p.qtContainer);
  widget.setVisible(widget.visible());
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  if(widget.state.layout && widget.state.layout->visible() == false) visible = false;
  qtWidget->setVisible(visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) qtWidget = new QWidget;
}
