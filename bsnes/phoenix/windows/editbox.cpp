void EditBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN |
    (editBox->wordWrap == false ? ES_AUTOHSCROLL : 0),
    editBox->x = x, editBox->y = y, editBox->width = width, editBox->height = height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  setText(text);
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
}

string EditBox::getText() {
  unsigned length = GetWindowTextLength(widget->window);
  wchar_t buffer[length + 1];
  GetWindowText(widget->window, buffer, length + 1);
  buffer[length] = 0;
  string text = utf8_t(buffer);
  text.replace("\r", "");
  return text;
}

void EditBox::setText(const string &text) {
  string output = text;
  output.replace("\r", "");
  output.replace("\n", "\r\n");
  object->locked = true;
  SetWindowText(widget->window, utf16_t(output));
  object->locked = false;
}

void EditBox::setCursorPosition(unsigned position) {
  Edit_SetSel(widget->window, position, position);
}

void EditBox::setEditable(bool editable) {
  SendMessage(widget->window, EM_SETREADONLY, editable == false, (LPARAM)0);
}

void EditBox::setWordWrap(bool wordWrap) {
  editBox->wordWrap = wordWrap;
  if(widget->window == 0) return;

  //ES_AUTOSCROLL options cannot be changed after control has been created;
  //so destroy the control and recreate it with desired options
  HWND hparent = GetParent(widget->window);
  Window *parent = (Window*)GetWindowLongPtr(hparent, GWLP_USERDATA);
  string text = getText();
  DestroyWindow(widget->window);
  create(*parent, editBox->x, editBox->y, editBox->width, editBox->height, text);
}

EditBox::EditBox() {
  editBox = new EditBox::Data;
  editBox->wordWrap = true;
}
