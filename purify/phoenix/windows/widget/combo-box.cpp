void pComboBox::append(const string &text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(text));
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) setSelection(0);
}

Geometry pComboBox::minimumGeometry() {
  unsigned maximumWidth = 0;
  for(auto &text : comboBox.state.text) maximumWidth = max(maximumWidth, pFont::geometry(hfont, text).width);
  return { 0, 0, maximumWidth + 24, pFont::geometry(hfont, " ").height + 10 };
}

void pComboBox::modify(unsigned row, const string &text) {
  locked = true;
  unsigned position = selection();
  SendMessage(hwnd, CB_DELETESTRING, row, 0);
  SendMessage(hwnd, CB_INSERTSTRING, row, (LPARAM)(wchar_t*)utf16_t(text));
  setSelection(position);
  locked = false;
}

void pComboBox::remove(unsigned row) {
  locked = true;
  unsigned position = selection();
  SendMessage(hwnd, CB_DELETESTRING, row, 0);
  if(position == row) setSelection(0);
  locked = false;
}

void pComboBox::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

unsigned pComboBox::selection() {
  return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void pComboBox::setSelection(unsigned row) {
  SendMessage(hwnd, CB_SETCURSEL, row, 0);
}

void pComboBox::constructor() {
  hwnd = CreateWindow(
    L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, 0, 0,
    parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&comboBox);
  setDefaultFont();
  for(auto &text : comboBox.state.text) append(text);
  setSelection(comboBox.state.selection);
  synchronize();
}

void pComboBox::destructor() {
  DestroyWindow(hwnd);
}

void pComboBox::orphan() {
  destructor();
  constructor();
}

void pComboBox::setGeometry(const Geometry &geometry) {
  SetWindowPos(hwnd, NULL, geometry.x, geometry.y, geometry.width, 1, SWP_NOZORDER);
  RECT rc;
  GetWindowRect(hwnd, &rc);
  unsigned adjustedHeight = geometry.height - ((rc.bottom - rc.top) - SendMessage(hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);
}
