void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindow(
    L"phoenix_label", L"",
    WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : os.os->proportionalFont), 0);
  setText(text);
}

void Label::setText(const char *text) {
  SetWindowText(widget->window, utf16_t(text));
}

//all of this for want of a STATIC SS_VCENTER flag ...
LRESULT CALLBACK Label_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Window *window_ptr = (Window*)GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA);
  if(!window_ptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Label *label_ptr = (Label*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!label_ptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  Window &window = *window_ptr;
  Label &label = *label_ptr;

  switch(msg) {
    case WM_ERASEBKGND: {
      if(window.window->brush == 0) break;
      RECT rc;
      GetClientRect(window.widget->window, &rc);
      PAINTSTRUCT ps;
      BeginPaint(window.widget->window, &ps);
      FillRect(ps.hdc, &rc, window.window->brush);
      EndPaint(window.widget->window, &ps);
      return TRUE;
    }

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);
      SelectObject(ps.hdc, label.widget->font);
      if(window.window->brush) {
        SetBkColor(ps.hdc, window.window->brushColor);
      } else {
        SetBkColor(ps.hdc, GetSysColor(COLOR_3DFACE));
      }
      RECT rc;
      GetClientRect(hwnd, &rc);
      unsigned length = GetWindowTextLength(hwnd) + 1;
      wchar_t text[length];
      GetWindowText(hwnd, text, length + 1);
      text[length] = 0;
      DrawText(ps.hdc, text, -1, &rc, DT_CALCRECT | DT_END_ELLIPSIS);
      unsigned height = rc.bottom;
      GetClientRect(hwnd, &rc);
      rc.top = (rc.bottom - height) / 2;
      rc.bottom = rc.top + height;
      DrawText(ps.hdc, text, -1, &rc, DT_LEFT | DT_END_ELLIPSIS);
      EndPaint(hwnd, &ps);
      InvalidateRect(hwnd, 0, false);
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
