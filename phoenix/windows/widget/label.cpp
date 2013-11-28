namespace phoenix {

Size pLabel::minimumSize() {
  Size size = pFont::size(hfont, label.state.text);
  return {size.width, size.height};
}

void pLabel::setText(string text) {
  SetWindowText(hwnd, utf16_t(text));
  InvalidateRect(hwnd, 0, false);
}

void pLabel::constructor() {
  hwnd = CreateWindow(L"phoenix_label", L"",
    WS_CHILD,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&label);
  setDefaultFont();
  setText(label.state.text);
  synchronize();
}

void pLabel::destructor() {
  DestroyWindow(hwnd);
}

void pLabel::orphan() {
  destructor();
  constructor();
}

static LRESULT CALLBACK Label_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  HWND parentHwnd = GetParent(hwnd);
  Label* label = (Label*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(label == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window* window = (Window*)label->Sizable::state.window;
  if(window == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_GETDLGCODE: return DLGC_STATIC | DLGC_WANTCHARS;
  case WM_ERASEBKGND: return TRUE;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    RECT rc;
    GetClientRect(hwnd, &rc);
    DrawThemeParentBackground(hwnd, ps.hdc, &rc);
    SetBkMode(ps.hdc, TRANSPARENT);
    SelectObject(ps.hdc, ((Widget*)label)->p.hfont);
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
    return FALSE;
  }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

}
