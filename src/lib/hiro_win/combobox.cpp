void pCombobox::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", "",
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, width, 200,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
}

void pCombobox::add_item(const char *text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)text);
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
