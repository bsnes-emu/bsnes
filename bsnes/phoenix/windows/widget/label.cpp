Geometry pLabel::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(label.state.text);
  return { 0, 0, geometry.width, geometry.height };
}

void pLabel::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
  InvalidateRect(hwnd, 0, false);
}

void pLabel::constructor() {
  setParent(Window::None);
}

void pLabel::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindow(L"phoenix_label", L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&label);
  setDefaultFont();
  setText(label.state.text);
  widget.setVisible(widget.visible());
}

static LRESULT CALLBACK Label_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Window *window = (Window*)GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA);
  Label *label = (Label*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!window || !label) return DefWindowProc(hwnd, msg, wparam, lparam);

  if(msg == WM_ERASEBKGND) {
    //background is erased during WM_PAINT to prevent flickering
    return TRUE;
  }

  if(msg == WM_PAINT) {
    PAINTSTRUCT ps;
    RECT rc;
    BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    FillRect(ps.hdc, &rc, window->p.brush ? window->p.brush : GetSysColorBrush(COLOR_3DFACE));
    SetBkColor(ps.hdc, window->p.brush ? window->p.brushColor : GetSysColor(COLOR_3DFACE));
    SelectObject(ps.hdc, ((Widget*)label)->state.font ? ((Widget*)label)->state.font->p.hfont : pOS::state->defaultFont.p.hfont);
    unsigned length = GetWindowTextLength(hwnd);
    wchar_t text[length + 1];
    GetWindowText(hwnd, text, length + 1);
    text[length] = 0;
    DrawText(ps.hdc, text, -1, &rc, DT_CALCRECT | DT_END_ELLIPSIS);
    unsigned height = rc.bottom;
    GetClientRect(hwnd, &rc);
    rc.top = (rc.bottom - height) / 2;
    rc.bottom = rc.top + height;
    DrawText(ps.hdc, text, -1, &rc, DT_LEFT | DT_END_ELLIPSIS);
    EndPaint(hwnd, &ps);
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
