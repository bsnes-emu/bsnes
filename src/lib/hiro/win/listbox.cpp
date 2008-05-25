void pListbox::create(uint style, uint width, uint height, const char *columns, const char *text) {
  bool header = style & Listbox::Header;
  unsigned hscroll = (style & Listbox::HorizontalScrollAlways) ? WS_HSCROLL :
                     (style & Listbox::HorizontalScrollNever) ? 0 :
                     0;
  unsigned vscroll = (style & Listbox::VerticalScrollAlways) ? WS_VSCROLL :
                     (style & Listbox::VerticalScrollNever) ? 0 :
                     0;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE |
    LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | vscroll | hscroll |
    (header ? 0 : LVS_NOCOLUMNHEADER),
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

  lstring list;
  split(list, "\t", columns ? columns : "");
  column_count = count(list);
  for(unsigned i = 0; i < count(list); i++) {
    LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.iSubItem = count(list);
    utf16 ulist(list[i]);
    column.pszText = ulist;
    ListView_InsertColumn(hwnd, i, &column);
  }

  if(text && *text) {
    split(list, "\n", text);
    for(unsigned i = 0; i < count(list); i++) add_item(list[i]);
  }
  autosize_columns();
}

void pListbox::autosize_columns() {
  for(unsigned i = 0; i < column_count; i++) {
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
  unsigned pos = ListView_GetItemCount(hwnd);
  item.mask = LVIF_TEXT;
  item.iItem = pos;
  item.iSubItem = 0;
  utf16 wtext(list[0]);
  item.pszText = wtext;
  ListView_InsertItem(hwnd, &item);

  for(unsigned i = 1; i < count(list); i++) {
    utf16 wtext(list[i]);
    ListView_SetItemText(hwnd, pos, i, wtext);
  }
}

void pListbox::set_item(uint index, const char *text) {
  lstring list;
  split(list, "\t", text ? text : "");
  for(unsigned i = 0; i < count(list); i++) {
    utf16 wtext(list[i]);
    ListView_SetItemText(hwnd, index, i, wtext);
  }
}

int pListbox::get_selection() {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned i = 0; i < count; i++) {
    if(ListView_GetItemState(hwnd, i, LVIS_SELECTED)) return i;
  }
  return -1;
}

void pListbox::set_selection(int index) {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned i = 0; i < count; i++) {
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
