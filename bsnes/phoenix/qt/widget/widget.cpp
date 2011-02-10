Geometry Widget::geometry() {
  return {
    widget->widget->x(), widget->widget->y(),
    widget->widget->width(), widget->widget->height()
  };
}

void Widget::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  widget->widget->setGeometry(x, y, width, height);
}

void Widget::setFont(Font &font) {
  widget->font = &font;
  widget->widget->setFont(*font.font);
}

bool Widget::visible() {
  return widget->widget->isVisible();
}

void Widget::setVisible(bool visible) {
  widget->widget->setVisible(visible);
}

bool Widget::enabled() {
  return widget->widget->isEnabled();
}

void Widget::setEnabled(bool enabled) {
  widget->widget->setEnabled(enabled);
}

bool Widget::focused() {
  return widget->widget->hasFocus();
}

void Widget::setFocused() {
  widget->widget->setFocus(Qt::OtherFocusReason);
}

Widget::Widget() {
  widget = new Widget::Data(*this);
}
