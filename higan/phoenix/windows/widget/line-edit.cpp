namespace phoenix {

Size pLineEdit::minimumSize() {
  Size size = pFont::size(hfont, lineEdit.state.text);
  return {size.width + 12, size.height + 10};
}

void pLineEdit::setEditable(bool editable) {
  SendMessage(hwnd, EM_SETREADONLY, editable == false, 0);
}

void pLineEdit::setText(const string &text) {
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
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
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

}
