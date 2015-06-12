#if defined(Hiro_LineEdit)

namespace hiro {

auto pLineEdit::construct() -> void {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setText(state().text);
}

auto pLineEdit::destruct() -> void {
  if(backgroundBrush) { DeleteObject(backgroundBrush); backgroundBrush = 0; }
  DestroyWindow(hwnd);
}

auto pLineEdit::minimumSize() const -> Size {
  auto size = pFont::size(hfont, state().text);
  return {size.width() + 12, size.height() + 10};
}

auto pLineEdit::setBackgroundColor(Color color) -> void {
  if(backgroundBrush) { DeleteObject(backgroundBrush); backgroundBrush = 0; }
  backgroundBrush = CreateSolidBrush(color ? CreateRGB(color) : GetSysColor(COLOR_WINDOW));
}

auto pLineEdit::setEditable(bool editable) -> void {
  SendMessage(hwnd, EM_SETREADONLY, editable == false, 0);
}

auto pLineEdit::setForegroundColor(Color color) -> void {
}

auto pLineEdit::setText(const string& text) -> void {
  lock();
  SetWindowText(hwnd, utf16_t(text));
  unlock();
}

auto pLineEdit::onChange() -> void {
  state().text = _text();
  if(!locked()) self().doChange();
}

auto pLineEdit::_text() -> string {
  unsigned length = GetWindowTextLength(hwnd);
  wchar_t text[length + 1];
  GetWindowText(hwnd, text, length + 1);
  text[length] = 0;
  return (const char*)utf8_t(text);
}

}

#endif
