void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  widget->window = CreateWindow(
    L"phoenix_label", L"",
    WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
  setText(text);
}

void Label::setText(const string &text) {
  SetWindowText(widget->window, utf16_t(text));
  InvalidateRect(widget->window, 0, false);
}

//all of this for want of a STATIC SS_VCENTER flag ...
LRESULT CALLBACK Label_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Window *window_ptr = (Window*)GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA);
  if(!window_ptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Label *label_ptr = (Label*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!label_ptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window &window = *window_ptr;
  Label &label = *label_ptr;

  switch(msg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      RECT rc;
      BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &rc);
      FillRect(ps.hdc, &rc, window.window->brush ? window.window->brush : GetSysColorBrush(COLOR_3DFACE));
      SetBkColor(ps.hdc, window.window->brush ? window.window->brushColor : GetSysColor(COLOR_3DFACE));
      SelectObject(ps.hdc, label.widget->font);
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
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
