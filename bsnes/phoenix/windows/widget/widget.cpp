bool pWidget::enabled() {
  return false;
}

void pWidget::setEnabled(bool enabled) {
}

void pWidget::setFocused() {
}

void pWidget::setFont(Font &font) {
}

void pWidget::setGeometry(const Geometry &geometry) {
  SetWindowPos(hwnd, NULL, geometry.x, geometry.y, geometry.width, geometry.height, SWP_NOZORDER);
}

void pWidget::setVisible(bool visible) {
}

void pWidget::constructor() {
  hwnd = 0;
}

void pWidget::setDefaultFont() {
  if(widget.state.font) {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)widget.state.font->p.hfont, 0);
  } else {
    SendMessage(hwnd, WM_SETFONT, (WPARAM)pOS::state->defaultFont.p.hfont, 0);
  }
}

void pWidget::setParent(Window &parent) {
}
