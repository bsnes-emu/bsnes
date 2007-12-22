void pListbox::create(uint style, uint width, uint height, const char *columns, const char *text) {
bool header = style & Listbox::Header;
uint hscroll = (style & Listbox::HorizontalScrollAlways) ? WS_HSCROLL :
               (style & Listbox::HorizontalScrollNever) ? 0 :
               0;
uint vscroll = (style & Listbox::VerticalScrollAlways) ? WS_VSCROLL :
               (style & Listbox::VerticalScrollNever) ? 0 :
               0;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
    WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | vscroll | hscroll |
    (header ? 0 : LVS_NOCOLUMNHEADER),
    0, 0, width, height,
    miu().p.default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)miu().p.default_font, 0);
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

lstring list;
  split(list, "\t", columns ? columns : "");
  column_count = count(list);
  for(uint i = 0; i < count(list); i++) {
  LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.iSubItem = count(list);
    column.pszText = (LPSTR)list[i]();
    ListView_InsertColumn(hwnd, i, &column);
  }

  if(text && *text) {
    split(list, "\n", text);
    for(uint i = 0; i < count(list); i++) add_item(list[i]);
  }
  autosize_columns();
}

void pListbox::autosize_columns() {
  for(uint i = 0; i < column_count; i++) {
    ListView_SetColumnWidth(hwnd, i, LVSCW_AUTOSIZE_USEHEADER);
  }
}

void pListbox::set_column_width(uint column, uint width) {
  ListView_SetColumnWidth(hwnd, column, width);
}

void pListbox::add_item(const char *text) {
lstring list;
  split(list, "\t", text ? text : "");
LVITEM item;
uint pos = ListView_GetItemCount(hwnd);
  item.mask = LVIF_TEXT;
  item.iItem = pos;
  item.iSubItem = 0;
  item.pszText = (LPSTR)list[0]();
  ListView_InsertItem(hwnd, &item);

  for(uint i = 1; i < count(list); i++) {
    ListView_SetItemText(hwnd, pos, i, (LPSTR)list[i]());
  }
}

void pListbox::set_item(uint index, const char *text) {
lstring list;
  split(list, "\t", text ? text : "");
  for(uint i = 0; i < count(list); i++) {
    ListView_SetItemText(hwnd, index, i, list[i]());
  }
}

int pListbox::get_selection() {
uint count = ListView_GetItemCount(hwnd);
  for(uint i = 0; i < count; i++) {
    if(ListView_GetItemState(hwnd, i, LVIS_SELECTED)) return i;
  }
  return -1;
}

void pListbox::set_selection(int index) {
uint count = ListView_GetItemCount(hwnd);
  for(uint i = 0; i < count; i++) {
    ListView_SetItemState(hwnd, i, LVIS_FOCUSED,  (i == index) ? LVIS_FOCUSED  : 0);
    ListView_SetItemState(hwnd, i, LVIS_SELECTED, (i == index) ? LVIS_SELECTED : 0);
  }
}

void pListbox::reset() {
  ListView_DeleteAllItems(hwnd);
}

pListbox::pListbox(Listbox &self_) : pFormControl(self_), self(self_) {
  column_count = 0;
}
