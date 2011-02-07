bool CheckBox::checked() {
  return SendMessage(widget->window, BM_GETCHECK, 0, 0);
}

void CheckBox::setChecked(bool checked) {
  SendMessage(widget->window, BM_SETCHECK, (WPARAM)checked, 0);
}

void CheckBox::setText(const string &text) {
  SetWindowText(widget->window, utf16_t(text));
}

CheckBox::CheckBox() {
  widget->window = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_CHECKBOX,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}
