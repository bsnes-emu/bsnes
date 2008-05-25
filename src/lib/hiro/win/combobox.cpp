void pCombobox::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, width, 200,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);

  //fix combobox height
  //CreateWindow height parameter represents dropdown window height; auto-sizes control based on current font setting
  //GetWindowRect returns true height (~22px), CB_GETITEMHEIGHT returns text height (~16px)
  //difference = WindowHeight - CB_GETITEMHEIGHT
  //CB_SETITEMHEIGHT to height - difference, so that height matches requested height perfectly
  RECT rc;
  GetWindowRect(hwnd, &rc);
  unsigned adjusted_height = height - ((rc.bottom - rc.top) - SendMessage(hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjusted_height);
}

void pCombobox::add_item(const char *text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16(text));
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) set_selection(0);
}

int pCombobox::get_selection() {
  return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void pCombobox::set_selection(int index) {
  SendMessage(hwnd, CB_SETCURSEL, combobox_selection = index, 0);
}

void pCombobox::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

pCombobox::pCombobox(Combobox &self_) : pFormControl(self_), self(self_) {
  combobox_selection = 0;
}
