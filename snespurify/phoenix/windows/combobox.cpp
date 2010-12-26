void ComboBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  widget->window = CreateWindowEx(
    0, L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    x, y, width, 200,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );

  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);

  //CreateWindow height parameter is the height of the expanded list box;
  //need additional code to override default ComboBox control height
  RECT rc;
  GetWindowRect(widget->window, &rc);
  unsigned adjustedHeight = height - ((rc.bottom - rc.top) - SendMessage(widget->window, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(widget->window, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);

  if(*text) {
    lstring list;
    list.split("\n", text);
    foreach(item, list) addItem(item);
  }
}

void ComboBox::reset() {
  SendMessage(widget->window, CB_RESETCONTENT, 0, 0);
}

void ComboBox::addItem(const string &text) {
  SendMessage(widget->window, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(text));
  if(SendMessage(widget->window, CB_GETCOUNT, 0, 0) == 1) setSelection(0);
}

unsigned ComboBox::selection() {
  return SendMessage(widget->window, CB_GETCURSEL, 0, 0);
}

void ComboBox::setSelection(unsigned row) {
  SendMessage(widget->window, CB_SETCURSEL, comboBox->selection = row, 0);
}

ComboBox::ComboBox() {
  comboBox = new ComboBox::Data;
  comboBox->selection = 0;
}
