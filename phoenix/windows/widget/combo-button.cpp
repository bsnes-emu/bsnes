namespace phoenix {

void pComboButton::append(string text) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(text));
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) setSelection(0);
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto& text : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(hfont, text).width);
  return {maximumWidth + 24, pFont::size(hfont, " ").height + 10};
}

void pComboButton::remove(unsigned selection) {
  locked = true;
  SendMessage(hwnd, CB_DELETESTRING, selection, 0);
  locked = false;

  if(selection == comboButton.state.selection) comboButton.setSelection(0);
}

void pComboButton::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

void pComboButton::setGeometry(Geometry geometry) {
  //height = minimum drop-down list height; use CB_SETITEMHEIGHT to control actual widget height
  pWidget::setGeometry({geometry.x, geometry.y, geometry.width, 1});
  RECT rc;
  GetWindowRect(hwnd, &rc);
  unsigned adjustedHeight = geometry.height - ((rc.bottom - rc.top) - SendMessage(hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);
}

void pComboButton::setSelection(unsigned selection) {
  SendMessage(hwnd, CB_SETCURSEL, selection, 0);
}

void pComboButton::setText(unsigned selection, string text) {
  locked = true;
  SendMessage(hwnd, CB_DELETESTRING, selection, 0);
  SendMessage(hwnd, CB_INSERTSTRING, selection, (LPARAM)(wchar_t*)utf16_t(text));
  setSelection(comboButton.state.selection);
  locked = false;
}

void pComboButton::constructor() {
  hwnd = CreateWindow(
    L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, 0, 0,
    parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&comboButton);
  setDefaultFont();
  for(auto& text : comboButton.state.text) append(text);
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

void pComboButton::onChange() {
  unsigned selection = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
  if(selection == comboButton.state.selection) return;
  comboButton.state.selection = selection;
  if(comboButton.onChange) comboButton.onChange();
}

}
