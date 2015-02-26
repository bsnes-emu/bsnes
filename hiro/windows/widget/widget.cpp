namespace phoenix {

bool pWidget::focused() {
  return GetFocus() == hwnd;
}

Size pWidget::minimumSize() {
  return {0, 0};
}

void pWidget::setEnabled(bool enabled) {
  if(!widget.parent()) enabled = false;
  if(widget.state.abstract) enabled = false;
  if(!widget.enabledToAll()) enabled = false;
  EnableWindow(hwnd, enabled);
}

void pWidget::setFocused() {
  SetFocus(hwnd);
}

void pWidget::setFont(string font) {
  if(hfont) DeleteObject(hfont);
  hfont = pFont::create(font);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

void pWidget::setGeometry(Geometry geometry) {
  if(GetParentWidget(&sizable)) {
    Position displacement = GetParentWidget(&sizable)->state.geometry.position();
    geometry.x -= displacement.x;
    geometry.y -= displacement.y;
  }
  SetWindowPos(hwnd, NULL, geometry.x, geometry.y, geometry.width, geometry.height, SWP_NOZORDER);
  if(widget.onSize) widget.onSize();
}

void pWidget::setVisible(bool visible) {
  if(!widget.parent()) visible = false;
  if(widget.state.abstract) visible = false;
  if(!widget.visibleToAll()) visible = false;
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
}

void pWidget::constructor() {
  hfont = pFont::create(Font::sans(8));
  if(widget.state.abstract) {
    hwnd = CreateWindow(L"phoenix_label", L"",
    WS_CHILD,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
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
  if(description.empty()) description = Font::sans(8);
  hfont = pFont::create(description);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, 0);
}

//calling Widget::setParent destroys widget and re-creates it:
//need to re-apply visiblity and enabled status; called by each subclassed setParent() function
//constructors are called top-down, so set each widget to the top of the z-order (so children appear on top of parents)
void pWidget::synchronize() {
  widget.setEnabled(widget.enabled());
  widget.setVisible(widget.visible());
  SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

}
