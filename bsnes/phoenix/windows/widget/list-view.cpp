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
}

void pListView::autoSizeColumns() {
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
}

void pListView::reset() {
  ListView_DeleteAllItems(hwnd);
}

bool pListView::selected() {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned n = 0; n < count; n++) {
    if(ListView_GetItemState(hwnd, n, LVIS_SELECTED)) return true;
  }
  return false;
}

unsigned pListView::selection() {
  unsigned count = ListView_GetItemCount(hwnd);
  for(unsigned n = 0; n < count; n++) {
    if(ListView_GetItemState(hwnd, n, LVIS_SELECTED)) return n;
  }
  return listView.state.selection;
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
  autoSizeColumns();
}

void pListView::setHeaderVisible(bool visible) {
  SetWindowLong(
    hwnd, GWL_STYLE,
    (GetWindowLong(hwnd, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
    (visible ? 0 : LVS_NOCOLUMNHEADER)
  );
}

void pListView::setSelected(bool selected) {
  locked = true;
  if(selected == false) {
    ListView_SetItemState(hwnd, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);
  } else {
    setSelection(listView.state.selection);
  }
  locked = false;
}

void pListView::setSelection(unsigned row) {
  locked = true;
  ListView_SetItemState(hwnd, row, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
  locked = false;
}

void pListView::constructor() {
  lostFocus = false;
  setParent(Window::None);
  listView.setHeaderText("");
}

void pListView::setGeometry(const Geometry &geometry) {
  pWidget::setGeometry(geometry);
  autoSizeColumns();
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
  if(listView.state.selected) setSelection(listView.state.selection);
  autoSizeColumns();
  widget.setVisible(widget.visible());
}
