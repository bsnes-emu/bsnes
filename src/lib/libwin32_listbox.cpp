void Listbox::AddItem(const char *text) {
  SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)text);
}

void Listbox::DeleteItem(uint id) {
  SendMessage(hwnd, LB_DELETESTRING, id, 0);
}

void Listbox::DeleteAllItems() {
  SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
}

uint Listbox::GetItemCount() {
  return SendMessage(hwnd, LB_GETCOUNT, 0, 0);
}

void Listbox::SetSelection(uint id) {
  SendMessage(hwnd, LB_SETCURSEL, id, 0);
}

int Listbox::GetSelection() {
  return SendMessage(hwnd, LB_GETCURSEL, 0, 0);
}

bool Listbox::Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text) {
  if(!parent_window)return false;

  parent = parent_window;
  id     = CONTROLID_INDEX + parent->control_count;
  type   = LISTBOX;
  state.ws     = WS_CHILD | LBS_NOTIFY;
  state.es     = 0;
  state.x      = x;
  state.y      = y;
  state.width  = width;
  state.height = height;

string part;
  ParseStyleParam(style, part);
  for(int i = 0; i < count(part); i++) {
    if(strmatch(part[i], "visible"))state.ws |= WS_VISIBLE;
    if(strmatch(part[i], "disabled"))state.ws |= WS_DISABLED;
    if(strmatch(part[i], "border"))state.ws |= WS_BORDER;
    if(strmatch(part[i], "raised"))state.ws |= WS_DLGFRAME;

    if(strmatch(part[i], "sunken"))state.es |= WS_EX_STATICEDGE;
    if(strmatch(part[i], "edge"))state.es |= WS_EX_CLIENTEDGE;
  }

  hwnd = CreateWindowEx(state.es, "LISTBOX", text, state.ws,
    state.x, state.y, state.width, state.height,
    parent->hwnd, (HMENU)id, GetModuleHandle(0), 0);
  if(!hwnd)return false;

  if(strmatch(text, "") == false) {
  string t;
    split(t, "|", text);
    for(int i = 0; i < ::count(t); i++) {
      AddItem(strptr(t[i]));
    }
  }
  SetSelection(0);

  PostCreate();
  return true;
}
