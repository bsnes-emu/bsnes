bool pWidget::enabled() {
  return IsWindowEnabled(hwnd);
}

Geometry pWidget::minimumGeometry() {
  return { 0, 0, 0, 0 };
}

void pWidget::setEnabled(bool enabled) {
  if(widget.state.abstract) enabled = false;
  if(sizable.state.layout && sizable.state.layout->enabled() == false) enabled = false;
  EnableWindow(hwnd, enabled);
}

void pWidget::setFocused() {
  SetFocus(hwnd);
}

void pWidget::setFont(const string &font) {
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(font);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

void pWidget::setGeometry(const Geometry &geometry) {
  SetWindowPos(hwnd, NULL, geometry.x, geometry.y, geometry.width, geometry.height, SWP_NOZORDER);
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  if(sizable.state.layout && sizable.state.layout->visible() == false) visible = false;
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
}

void pWidget::constructor() {
  hfont = pFont::create("Tahoma, 8");
  if(widget.state.abstract) {
    hwnd = CreateWindow(L"phoenix_label", L"", WS_CHILD, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&widget);
  }
}

void pWidget::destructor() {
  if(widget.state.abstract) {
    DestroyWindow(hwnd);
  }
}

void pWidget::orphan() {
  destructor();
  constructor();
}

void pWidget::setDefaultFont() {
  string description = widget.state.font;
  if(description == "") description = "Tahoma, 8";
  hfont = pFont::create(description);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

//calling Widget::setParent destroys widget and re-creates it:
//need to re-apply visiblity and enabled status; called by each subclassed setParent() function
void pWidget::synchronize() {
  widget.setEnabled(widget.enabled());
  widget.setVisible(widget.visible());
}
