#if defined(Hiro_TextEdit)

namespace hiro {

auto pTextEdit::construct() -> void {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | (!state().wordWrap ? WS_HSCROLL | ES_AUTOHSCROLL : 0),
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setText(state().text);
  setCursor(state().cursor);
}

auto pTextEdit::destruct() -> void {
  state().text = text();
  if(backgroundBrush) { DeleteObject(backgroundBrush); backgroundBrush = 0; }
  DestroyWindow(hwnd);
}

auto pTextEdit::setBackgroundColor(Color color) -> void {
  if(backgroundBrush) { DeleteObject(backgroundBrush); backgroundBrush = 0; }
  backgroundBrush = CreateSolidBrush(color ? CreateRGB(color) : GetSysColor(COLOR_WINDOW));
  InvalidateRect(hwnd, 0, true);
}

auto pTextEdit::setCursor(Cursor cursor) -> void {
  signed end = GetWindowTextLength(hwnd);
  signed offset = max(0, min(end, cursor.offset()));
  signed length = max(0, min(end, cursor.offset() + cursor.length()));
  Edit_SetSel(hwnd, offset, length);
  Edit_ScrollCaret(hwnd);
}

auto pTextEdit::setEditable(bool editable) -> void {
  SendMessage(hwnd, EM_SETREADONLY, editable == false, (LPARAM)0);
}

auto pTextEdit::setForegroundColor(Color color) -> void {
  InvalidateRect(hwnd, 0, true);
}

auto pTextEdit::setText(string text) -> void {
  auto lock = acquire();
  text.replace("\r", "");
  text.replace("\n", "\r\n");
  SetWindowText(hwnd, utf16_t(text));
}

auto pTextEdit::setWordWrap(bool wordWrap) -> void {
  //ES_AUTOHSCROLL cannot be changed after widget creation.
  //As a result, we must destroy and re-create widget to change this setting.
  reconstruct();
}

auto pTextEdit::text() const -> string {
  unsigned length = GetWindowTextLength(hwnd);
  wchar_t buffer[length + 1];
  GetWindowText(hwnd, buffer, length + 1);
  buffer[length] = 0;
  string text = (const char*)utf8_t(buffer);
  text.replace("\r", "");
  return text;
}

//

auto pTextEdit::onChange() -> void {
  if(!locked()) self().doChange();
}

auto pTextEdit::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_KEYDOWN) {
    if(wparam == 'A' && GetKeyState(VK_CONTROL) < 0) {
      //Ctrl+A = select all text
      //note: this is not a standard accelerator on Windows
      Edit_SetSel(hwnd, 0, ~0);
      return true;
    } else if(wparam == 'V' && GetKeyState(VK_CONTROL) < 0) {
      //Ctrl+V = paste text
      //note: this formats Unix (LF) and OS9 (CR) line-endings to Windows (CR+LF) line-endings
      //this is necessary as the EDIT control only supports Windows line-endings
      OpenClipboard(hwnd);
      if(auto handle = GetClipboardData(CF_UNICODETEXT)) {
        if(auto text = (wchar_t*)GlobalLock(handle)) {
          string data = (const char*)utf8_t(text);
          data.replace("\r\n", "\n");
          data.replace("\r", "\n");
          data.replace("\n", "\r\n");
          GlobalUnlock(handle);
          utf16_t output(data);
          if(auto resource = GlobalAlloc(GMEM_MOVEABLE, (wcslen(output) + 1) * sizeof(wchar_t))) {
            if(auto write = (wchar_t*)GlobalLock(resource)) {
              wcscpy(write, output);
              GlobalUnlock(write);
              if(SetClipboardData(CF_UNICODETEXT, resource) == nullptr) {
                GlobalFree(resource);
              }
            }
          }
        }
      }
      CloseClipboard();
    }
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

}

#endif
