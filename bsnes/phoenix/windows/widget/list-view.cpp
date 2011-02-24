void pListView::append(const lstring &list) {
  wchar_t empty[] = L"";
  unsigned row = ListView_GetItemCount(hwnd);
  LVITEM item;
  item.mask = LVIF_TEXT;
  item.iItem = row;
  item.iSubItem = 0;
  item.pszText = empty;
  locked = true;
  ListView_InsertItem(hwnd, &item);
  locked = false;
  foreach(text, list, n) {
    utf16_t wtext(text);
    ListView_SetItemText(hwnd, row, n, wtext);
  }
  //workaround: when there is only one column, the horizontal scrollbar will always appear without this
  if(listView.state.headerText.size() <= 1) ListView_SetColumnWidth(hwnd, 0, LVSCW_AUTOSIZE_USEHEADER);
}

void pListView::autosizeColumns() {
  for(unsigned n = 0; n < listView.state.headerText.size(); n++) {
    ListView_SetColumnWidth(hwnd, n, LVSCW_AUTOSIZE_USEHEADER);
  }
}

bool pListView::checked(unsigned row) {
  return ListView_GetCheckState(hwnd, row);
}

void pListView::modify(unsigned row, const lstring &list) {
  foreach(text, list, n) {
    utf16_t wtext(text);
    ListView_SetItemText(hwnd, row, n, wtext);
  }
  if(listView.state.headerText.size() <= 1) ListView_SetColumnWidth(hwnd, 0, LVSCW_AUTOSIZE_USEHEADER);
}

void pListView::modify(unsigned row, unsigned column, const string &text) {
  utf16_t wtext(text);
  ListView_SetItemText(hwnd, row, column, wtext);
  if(listView.state.headerText.size() <= 1) ListView_SetColumnWidth(hwnd, 0, LVSCW_AUTOSIZE_USEHEADER);
}

void pListView::reset() {
  ListView_DeleteAllItems(hwnd);
}

optional<unsigned> pListView::selection() {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned n = 0; n < count; n++) {
    if(ListView_GetItemState(hwnd, n, LVIS_SELECTED)) return { true, n };
  }
  return { false, 0 };
}

void pListView::setCheckable(bool checkable) {
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT | (checkable ? LVS_EX_CHECKBOXES : 0));
}

void pListView::setChecked(unsigned row, bool checked) {
  locked = true;
  ListView_SetCheckState(hwnd, row, checked);
  locked = false;
}

void pListView::setHeaderText(const lstring &list) {
  while(ListView_DeleteColumn(hwnd, 0));

  lstring headers = list;
  if(headers.size() == 0) headers.append("");  //must have at least one column

  foreach(text, headers, n) {
    LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.iSubItem = n;
    utf16_t headerText(text);
    column.pszText = headerText;
    ListView_InsertColumn(hwnd, n, &column);
  }
  autosizeColumns();
}

void pListView::setHeaderVisible(bool visible) {
  SetWindowLong(
    hwnd, GWL_STYLE,
    (GetWindowLong(hwnd, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
    (visible ? 0 : LVS_NOCOLUMNHEADER)
  );
}

void pListView::setSelection(unsigned row) {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned n = 0; n < count; n++) {
    ListView_SetItemState(hwnd, n, LVIS_FOCUSED,  (n == row ? LVIS_FOCUSED  : 0));
    ListView_SetItemState(hwnd, n, LVIS_SELECTED, (n == row ? LVIS_SELECTED : 0));
  }
}

void pListView::constructor() {
  lostFocus = false;
  setParent(Window::None);
}

void pListView::setGeometry(const Geometry &geometry) {
  pWidget::setGeometry(geometry);
  autosizeColumns();
}

void pListView::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER,
    0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&listView);
  setDefaultFont();
  setHeaderText(listView.state.headerText);
  setHeaderVisible(listView.state.headerVisible);
  setCheckable(listView.state.checkable);
  foreach(text, listView.state.text) append(text);
  foreach(checked, listView.state.checked, n) setChecked(n, checked);
  if(auto selection = listView.state.selection) setSelection(selection());
  autosizeColumns();
}
