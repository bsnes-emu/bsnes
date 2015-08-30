#if defined(Hiro_TextEdit)

namespace hiro {

auto pTextEdit::construct() -> void {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | (!state().wordWrap ? WS_HSCROLL | ES_AUTOHSCROLL : 0),
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
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
}

auto pTextEdit::setText(string text) -> void {
  lock();
  text.replace("\r", "");
  text.replace("\n", "\r\n");
  SetWindowText(hwnd, utf16_t(text));
  unlock();
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

auto pTextEdit::onChange() -> void {
  if(!locked()) self().doChange();
}

}

#endif
