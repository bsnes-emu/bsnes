unsigned ListView_GetColumnCount(HWND hwnd) {
  unsigned count = 0;
  LVCOLUMN column;
  column.mask = LVCF_WIDTH;
  while(ListView_GetColumn(hwnd, count++, &column));
  return --count;
}

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
  for(unsigned column = 0; column < list.size(); column++) {
    utf16_t wtext(list(column, ""));
    ListView_SetItemText(hwnd, row, column, wtext);
  }
}

void pListView::autoSizeColumns() {
  unsigned columns = ListView_GetColumnCount(hwnd);
  for(unsigned n = 0; n < columns; n++) {
    ListView_SetColumnWidth(hwnd, n, LVSCW_AUTOSIZE_USEHEADER);
  }
}

bool pListView::checked(unsigned row) {
  return ListView_GetCheckState(hwnd, row);
}

void pListView::modify(unsigned row, const lstring &list) {
  for(unsigned n = 0; n < list.size(); n++) {
    utf16_t wtext(list(n, ""));
    ListView_SetItemText(hwnd, row, n, wtext);
  }
}

void pListView::remove(unsigned row) {
  ListView_DeleteItem(hwnd, row);
  setImageList();
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
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | (checkable ? LVS_EX_CHECKBOXES : 0));
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

  for(unsigned n = 0; n < headers.size(); n++) {
    LVCOLUMN column;
    column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.iSubItem = n;
    utf16_t headerText(headers[n]);
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

void pListView::setImage(unsigned row, unsigned column, const image &image) {
  setImageList();
}

void pListView::setSelected(bool selected) {
  locked = true;
  lostFocus = false;
  if(selected == false) {
    ListView_SetItemState(hwnd, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);
  } else {
    setSelection(listView.state.selection);
  }
  locked = false;
}

void pListView::setSelection(unsigned row) {
  locked = true;
  lostFocus = false;
  ListView_SetItemState(hwnd, row, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
  locked = false;
}

void pListView::constructor() {
  lostFocus = false;
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
    WS_CHILD | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&listView);
  setDefaultFont();
  setHeaderText(listView.state.headerText);
  setHeaderVisible(listView.state.headerVisible);
  setCheckable(listView.state.checkable);
  for(auto &text : listView.state.text) append(text);
  for(unsigned n = 0; n < listView.state.checked.size(); n++) setChecked(n, listView.state.checked[n]);
  setImageList();
  if(listView.state.selected) setSelection(listView.state.selection);
  autoSizeColumns();
  synchronize();
}

void pListView::destructor() {
  DestroyWindow(hwnd);
}

void pListView::orphan() {
  destructor();
  constructor();
}

void pListView::setGeometry(const Geometry &geometry) {
  pWidget::setGeometry(geometry);
  autoSizeColumns();
}

void pListView::setImageList() {
  auto &list = listView.state.image;

  if(imageList) {
    ImageList_Destroy(imageList);
    imageList = nullptr;
  }

  bool found = false;
  for(auto &row : listView.state.image) {
    for(auto &column : row) {
      if(column.empty() == false) {
        found = true;
        break;
      }
    }
  }
  if(found == false) return;

  imageList = ImageList_Create(15, 15, ILC_COLOR32, 1, 0);
  nall::image image;
  image.allocate(15, 15);
  image.clear(GetSysColor(COLOR_WINDOW));
  ImageList_Add(imageList, CreateBitmap(image), NULL);

  for(unsigned row = 0; row < list.size(); row++) {
    for(unsigned column = 0; column < list(row).size(); column++) {
      nall::image image = list(row)(column);
      if(image.empty()) continue;
      image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
      image.scale(15, 15, Interpolation::Linear);
      ImageList_Add(imageList, CreateBitmap(image), NULL);
    }
  }

  ListView_SetImageList(hwnd, imageList, LVSIL_SMALL);

  unsigned ID = 1;
  for(unsigned row = 0; row < list.size(); row++) {
    for(unsigned column = 0; column < list(row).size(); column++) {
      if(list(row)(column).empty()) continue;  //I_IMAGENONE does not work properly
      LVITEM item;
      item.mask = LVIF_IMAGE;
      item.iItem = row;
      item.iSubItem = column;
      item.iImage = ID++;
      ListView_SetItem(hwnd, &item);
    }
  }
}
