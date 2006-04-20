void Listview::AddColumn(uint align, uint width, const char *text) {
LVCOLUMN column;
  column.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
  column.fmt      = (align == RIGHT) ? LVCFMT_RIGHT : LVCFMT_LEFT;
  column.iSubItem = column_count;
  column.pszText  = (LPSTR)text;
  column.cx       = width;
  ListView_InsertColumn(hwnd, column_count++, &column);
}

void Listview::AddItem(const char *text) {
string t;
  split(t, "|", text);

LVITEM item;
uint pos = ListView_GetItemCount(hwnd);
  item.mask     = LVIF_TEXT;
  item.iItem    = pos;
  item.iSubItem = 0;
  item.pszText  = (LPSTR)strptr(t[0]);
  ListView_InsertItem(hwnd, &item);

  for(int i = 1; i < count(t); i++) {
    ListView_SetItemText(hwnd, pos, i, (LPSTR)strptr(t[i]));
  }
}

void Listview::SetItemText(uint id, const char *text) {
string t;
  split(t, "|", text);

  for(int i = 0; i < count(t); i++) {
    ListView_SetItemText(hwnd, id, i, (LPSTR)strptr(t[i]));
  }
}

void Listview::DeleteAllItems() {
  ListView_DeleteAllItems(hwnd);
}

uint Listview::GetItemCount() {
  return ListView_GetItemCount(hwnd);
}

void Listview::SetSelection(uint id) {
uint count = ListView_GetItemCount(hwnd);
  for(int i = 0; i < count; i++) {
  uint state = ListView_GetItemState(hwnd, i, LVIS_FOCUSED);
    ListView_SetItemState(hwnd, i, LVIS_FOCUSED,  (i == id) ? LVIS_FOCUSED  : 0);
    ListView_SetItemState(hwnd, i, LVIS_SELECTED, (i == id) ? LVIS_SELECTED : 0);
  }
}

int Listview::GetSelection() {
uint count = ListView_GetItemCount(hwnd);
  for(int i = 0; i < count; i++) {
    if(ListView_GetItemState(hwnd, i, LVIS_SELECTED))return i;
  }
  return NONE;
}

bool Listview::Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text) {
  if(!parent_window)return false;

  parent = parent_window;
  id     = CONTROLID_INDEX + parent->control_count;
  type   = LISTVIEW;
  state.ws     = WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
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

  hwnd = CreateWindowEx(state.es, WC_LISTVIEW, text, state.ws,
    state.x, state.y, state.width, state.height,
    parent->hwnd, (HMENU)id, GetModuleHandle(0), 0);
  if(!hwnd)return false;

  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

  PostCreate();
  return true;
}
