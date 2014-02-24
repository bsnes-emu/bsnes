namespace phoenix {

Size pLineEdit::minimumSize() {
  Size size = pFont::size(hfont, lineEdit.state.text);
  return {size.width + 12, size.height + 10};
}

void pLineEdit::setBackgroundColor(Color color) {
}

void pLineEdit::setEditable(bool editable) {
  SendMessage(hwnd, EM_SETREADONLY, editable == false, 0);
}

void pLineEdit::setForegroundColor(Color color) {
}

void pLineEdit::setText(string text) {
  locked = true;
  SetWindowText(hwnd, utf16_t(text));
  locked = false;
}

string pLineEdit::text() {
  unsigned length = GetWindowTextLength(hwnd);
  wchar_t text[length + 1];
  GetWindowText(hwnd, text, length + 1);
  text[length] = 0;
  return (const char*)utf8_t(text);
}

void pLineEdit::constructor() {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&lineEdit);
  setDefaultFont();
  setEditable(lineEdit.state.editable);
  setText(lineEdit.state.text);
  synchronize();
}

void pLineEdit::destructor() {
  lineEdit.state.text = text();
  DestroyWindow(hwnd);
}

void pLineEdit::orphan() {
  destructor();
  constructor();
}

void pLineEdit::onChange() {
  if(locked) return;
  if(lineEdit.onChange) lineEdit.onChange();
}

}
