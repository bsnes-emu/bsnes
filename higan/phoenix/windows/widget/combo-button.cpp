namespace phoenix {

void pComboButton::append(const string &text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(text));
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) setSelection(0);
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto &text : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(hfont, text).width);
  return {maximumWidth + 24, pFont::size(hfont, " ").height + 10};
}

void pComboButton::modify(unsigned row, const string &text) {
  locked = true;
  unsigned position = selection();
  SendMessage(hwnd, CB_DELETESTRING, row, 0);
  SendMessage(hwnd, CB_INSERTSTRING, row, (LPARAM)(wchar_t*)utf16_t(text));
  setSelection(position);
  locked = false;
}

void pComboButton::remove(unsigned row) {
  locked = true;
  unsigned position = selection();
  SendMessage(hwnd, CB_DELETESTRING, row, 0);
  if(position == row) setSelection(0);
  locked = false;
}

void pComboButton::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

unsigned pComboButton::selection() {
  return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void pComboButton::setSelection(unsigned row) {
  SendMessage(hwnd, CB_SETCURSEL, row, 0);
}

void pComboButton::constructor() {
  hwnd = CreateWindow(
    L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, 0, 0,
    parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&comboButton);
  setDefaultFont();
  for(auto &text : comboButton.state.text) append(text);
  setSelection(comboButton.state.selection);
  synchronize();
}

void pComboButton::destructor() {
  DestroyWindow(hwnd);
}

void pComboButton::orphan() {
  destructor();
  constructor();
}

void pComboButton::setGeometry(const Geometry &geometry) {
  SetWindowPos(hwnd, NULL, geometry.x, geometry.y, geometry.width, 1, SWP_NOZORDER);
  RECT rc;
  GetWindowRect(hwnd, &rc);
  unsigned adjustedHeight = geometry.height - ((rc.bottom - rc.top) - SendMessage(hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);
}

}
