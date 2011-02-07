void ListBox::setHeaderText(const string &text) {
  //delete all existing columns
  while(ListView_DeleteColumn(widget->window, 0));

  lstring list;
  list.split("\t", text);
  listBox->columns = list.size();
  for(unsigned i = 0; i < list.size(); i++) {
    LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.iSubItem = list.size();
    utf16_t text(list[i]);
    column.pszText = text;
    ListView_InsertColumn(widget->window, i, &column);
  }
  resizeColumnsToContent();
}

void ListBox::setHeaderVisible(bool headerVisible) {
  SetWindowLong(
    widget->window,
    GWL_STYLE,
    (GetWindowLong(widget->window, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
    (headerVisible == false ? LVS_NOCOLUMNHEADER : 0)
  );
}

void ListBox::setCheckable(bool checkable) {
  ListView_SetExtendedListViewStyle(widget->window, LVS_EX_FULLROWSELECT | (checkable ? LVS_EX_CHECKBOXES : 0));
}

void ListBox::reset() {
  ListView_DeleteAllItems(widget->window);
}

void ListBox::resizeColumnsToContent() {
  for(unsigned i = 0; i < listBox->columns; i++) {
    ListView_SetColumnWidth(widget->window, i, LVSCW_AUTOSIZE_USEHEADER);
  }
}

void ListBox::addItem(const string &text) {
  lstring list;
  list.split("\t", text);
  LVITEM item;
  unsigned row = ListView_GetItemCount(widget->window);
  item.mask = LVIF_TEXT;
  item.iItem = row;
  item.iSubItem = 0;
  utf16_t wtext(list[0]);
  item.pszText = wtext;
  object->locked = true;
  ListView_InsertItem(widget->window, &item);
  object->locked = false;
  for(unsigned i = 1; i < list.size(); i++) {
    utf16_t wtext(list[i]);
    ListView_SetItemText(widget->window, row, i, wtext);
  }

  //workaround: when there is only one column, the horizontal scrollbar will always appear without this
  if(listBox->columns == 1) ListView_SetColumnWidth(widget->window, 0, LVSCW_AUTOSIZE_USEHEADER);
}

void ListBox::setItem(unsigned row, const string &text) {
  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) {
    utf16_t wtext(list[i]);
    ListView_SetItemText(widget->window, row, i, wtext);
  }

  //workaround: when there is only one column, the horizontal scrollbar will always appear without this
  if(listBox->columns == 1) ListView_SetColumnWidth(widget->window, 0, LVSCW_AUTOSIZE_USEHEADER);
}

optional<unsigned> ListBox::selection() {
  unsigned count = ListView_GetItemCount(widget->window);
  for(unsigned i = 0; i < count; i++) {
    if(ListView_GetItemState(widget->window, i, LVIS_SELECTED)) return { true, i };
  }
  return { false, 0 };
}

void ListBox::setSelection(unsigned row) {
  unsigned count = ListView_GetItemCount(widget->window);
  for(unsigned i = 0; i < count; i++) {
    ListView_SetItemState(widget->window, i, LVIS_FOCUSED, (i == row ? LVIS_FOCUSED : 0));
    ListView_SetItemState(widget->window, i, LVIS_SELECTED, (i == row ? LVIS_SELECTED : 0));
  }
}

bool ListBox::checked(unsigned row) {
  return ListView_GetCheckState(widget->window, row);
}

void ListBox::setChecked(unsigned row, bool checked) {
  object->locked = true;
  ListView_SetCheckState(widget->window, row, checked);
  object->locked = false;
}

ListBox::ListBox() {
  listBox = new ListBox::Data;
  listBox->lostFocus = false;

  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE |
    LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  ListView_SetExtendedListViewStyle(widget->window, LVS_EX_FULLROWSELECT);
  setHeaderText("");
}
