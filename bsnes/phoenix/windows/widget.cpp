void Widget::setFont(Font &font) {
  widget->font = font.font->font;
  SendMessage(widget->window, WM_SETFONT, (WPARAM)font.font->font, 0);
}

bool Widget::visible() {
  return GetWindowLong(widget->window, GWL_STYLE) & WS_VISIBLE;
}

void Widget::setVisible(bool visible) {
  ShowWindow(widget->window, visible ? SW_SHOWNORMAL : SW_HIDE);
}

bool Widget::enabled() {
  return IsWindowEnabled(widget->window);
}

void Widget::setEnabled(bool enabled) {
  EnableWindow(widget->window, enabled);
}

bool Widget::focused() {
  return (GetForegroundWindow() == widget->window);
}

void Widget::setFocused() {
  if(visible() == false) setVisible(true);
  SetFocus(widget->window);
}

Widget::Widget() {
  os.objects.append(this);
  widget = new Widget::Data;
  widget->window = 0;
  widget->font = os.os->proportionalFont;
}
