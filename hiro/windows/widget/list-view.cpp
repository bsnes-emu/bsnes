#if defined(Hiro_ListView)

namespace hiro {

static auto CALLBACK ListView_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto listView = dynamic_cast<mListView*>(object)) {
      if(auto self = listView->self()) {
        if(!listView->enabled(true)) {
          if(msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) {
            //WC_LISTVIEW responds to key messages even when its HWND is disabled
            //the control should be inactive when disabled; so we intercept the messages here
            return false;
          }
        }
        return self->windowProc(hwnd, msg, wparam, lparam);
      }
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pListView::construct() -> void {
  hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
    WS_CHILD | WS_TABSTOP | LVS_REPORT | LVS_SHOWSELALWAYS,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&ListView_windowProc);
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
  pWidget::_setState();
  setBackgroundColor(state().backgroundColor);
  setBatchable(state().batchable);
  setCheckable(state().checkable);
  setGridVisible(state().gridVisible);
  setHeaderVisible(state().headerVisible);
  setSortable(state().sortable);
  _setIcons();
  resizeColumns();
}

auto pListView::destruct() -> void {
  if(imageList) { ImageList_Destroy(imageList); imageList = 0; }
  DestroyWindow(hwnd);
}

auto pListView::append(sListViewColumn column) -> void {
  lock();
  wchar_t text[] = L"";
  LVCOLUMN lvColumn;
  lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
  lvColumn.fmt = LVCFMT_LEFT;
  lvColumn.iSubItem = column->offset();
  lvColumn.pszText = text;
  ListView_InsertColumn(hwnd, column->offset(), &lvColumn);
  if(auto self = column->self()) {
    self->_setState();
  }
  resizeColumns();
  unlock();
}

auto pListView::append(sListViewItem item) -> void {
  lock();
  wchar_t text[] = L"";
  LVITEM lvItem;
  lvItem.mask = LVIF_TEXT;
  lvItem.iItem = item->offset();
  lvItem.iSubItem = 0;
  lvItem.pszText = text;
  ListView_InsertItem(hwnd, &lvItem);
  if(auto self = item->self()) {
    self->setChecked(item->state.checked);
    self->setSelected(item->state.selected);
  }
  for(auto& cell : item->state.cells) {
    if(auto self = cell->self()) {
      self->_setState();
    }
  }
  unlock();
}

auto pListView::checkAll() -> void {
  for(auto& item : state().items) {
    item->self()->setChecked(true);
  }
}

auto pListView::remove(sListViewColumn column) -> void {
  lock();
  ListView_DeleteColumn(hwnd, column->offset());
  unlock();
}

auto pListView::remove(sListViewItem item) -> void {
  lock();
  ListView_DeleteItem(hwnd, item->offset());
  unlock();
}

auto pListView::reset() -> void {
  lock();
  ListView_DeleteAllItems(hwnd);
  LVCOLUMN lvColumn{LVCF_WIDTH};
  while(ListView_GetColumn(hwnd, 0, &lvColumn)) ListView_DeleteColumn(hwnd, 0);
  _setIcons();  //free icon resources
  unlock();
}

auto pListView::resizeColumns() -> void {
  lock();

  vector<signed> widths;
  signed minimumWidth = 0;
  signed expandable = 0;
  for(auto column : range(state().columns)) {
    signed width = _width(column);
    widths.append(width);
    minimumWidth += width;
    if(state().columns[column]->expandable()) expandable++;
  }

  signed maximumWidth = self().geometry().width() - 4;
  SCROLLBARINFO sbInfo{sizeof(SCROLLBARINFO)};
  if(GetScrollBarInfo(hwnd, OBJID_VSCROLL, &sbInfo)) {
    if(!(sbInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE)) {
      maximumWidth -= sbInfo.rcScrollBar.right - sbInfo.rcScrollBar.left;
    }
  }

  signed expandWidth = 0;
  if(expandable && maximumWidth > minimumWidth) {
    expandWidth = (maximumWidth - minimumWidth) / expandable;
  }

  for(auto column : range(state().columns)) {
    if(auto self = state().columns[column]->self()) {
      signed width = widths[column];
      if(self->state().expandable) width += expandWidth;
      self->_width = width;
      self->_setState();
    }
  }

  unlock();
}

auto pListView::selectAll() -> void {
  lock();
  ListView_SetItemState(hwnd, -1, LVIS_SELECTED, LVIS_SELECTED);
  unlock();
}

auto pListView::setBackgroundColor(Color color) -> void {
  if(!color) color = {255, 255, 255};
  ListView_SetBkColor(hwnd, RGB(color.red(), color.green(), color.blue()));
}

auto pListView::setBatchable(bool batchable) -> void {
  auto style = GetWindowLong(hwnd, GWL_STYLE);
  !batchable ? style |= LVS_SINGLESEL : style &=~ LVS_SINGLESEL;
  SetWindowLong(hwnd, GWL_STYLE, style);
}

auto pListView::setCheckable(bool checkable) -> void {
  auto style = ListView_GetExtendedListViewStyle(hwnd);
  checkable ? style |= LVS_EX_CHECKBOXES : style &=~ LVS_EX_CHECKBOXES;
  ListView_SetExtendedListViewStyle(hwnd, style);
}

auto pListView::setForegroundColor(Color color) -> void {
}

auto pListView::setGridVisible(bool visible) -> void {
  //rendered via onCustomDraw
}

auto pListView::setHeaderVisible(bool visible) -> void {
  auto style = GetWindowLong(hwnd, GWL_STYLE);
  !visible ? style |= LVS_NOCOLUMNHEADER : style &=~ LVS_NOCOLUMNHEADER;
  SetWindowLong(hwnd, GWL_STYLE, style);
}

auto pListView::setSortable(bool sortable) -> void {
  auto style = GetWindowLong(hwnd, GWL_STYLE);
  !sortable ? style |= LVS_NOSORTHEADER : style &=~ LVS_NOSORTHEADER;
  SetWindowLong(hwnd, GWL_STYLE, style);
}

auto pListView::uncheckAll() -> void {
  for(auto& item : state().items) {
    item->self()->setChecked(false);
  }
}

auto pListView::unselectAll() -> void {
  lock();
  ListView_SetItemState(hwnd, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);
  unlock();
}

auto pListView::onActivate(LPARAM lparam) -> void {
  auto nmlistview = (LPNMLISTVIEW)lparam;
  if(ListView_GetSelectedCount(hwnd) == 0) return;
  if(!locked()) {
    //LVN_ITEMACTIVATE is not re-entrant until DispatchMessage() completes
    //thus, we don't call self().doActivate() here
    PostMessageOnce(_parentHandle(), AppMessage::ListView_onActivate, 0, (LPARAM)&reference);
  }
}

auto pListView::onChange(LPARAM lparam) -> void {
  auto nmlistview = (LPNMLISTVIEW)lparam;
  if(!(nmlistview->uChanged & LVIF_STATE)) return;

  bool modified = false;
  for(auto& item : state().items) {
    bool selected = ListView_GetItemState(hwnd, item->offset(), LVIS_SELECTED) & LVIS_SELECTED;
    if(item->state.selected != selected) {
      modified = true;
      item->state.selected = selected;
    }
  }
  if(modified && !locked()) {
    //state event change messages are sent for every item
    //so when doing a batch select/deselect; this can generate several messages
    //we use a delayed AppMessage so that only one callback event is fired off
    PostMessageOnce(_parentHandle(), AppMessage::ListView_onChange, 0, (LPARAM)&reference);
  }

  if(!locked()) {
    unsigned row = nmlistview->iItem;
    unsigned mask = nmlistview->uNewState & LVIS_STATEIMAGEMASK;
    if(mask == 0x1000 || mask == 0x2000) {
      bool checked = mask == 0x2000;
      if(auto item = self().item(row)) {
        if(checked != item->state.checked) {  //WC_LISTVIEW sends this message twice
          item->state.checked = checked;
          self().doToggle(item);
        }
      }
    }
  }
}

auto pListView::onContext(LPARAM lparam) -> void {
  auto nmitemactivate = (LPNMITEMACTIVATE)lparam;
  return self().doContext();
}

auto pListView::onCustomDraw(LPARAM lparam) -> LRESULT {
  auto lvcd = (LPNMLVCUSTOMDRAW)lparam;

  switch(lvcd->nmcd.dwDrawStage) {
  default: return CDRF_DODEFAULT;
  case CDDS_PREPAINT: return CDRF_NOTIFYITEMDRAW;
  case CDDS_ITEMPREPAINT: return CDRF_NOTIFYSUBITEMDRAW | CDRF_NOTIFYPOSTPAINT;
  case CDDS_ITEMPREPAINT | CDDS_SUBITEM: return CDRF_SKIPDEFAULT;
  case CDDS_ITEMPOSTPAINT: {
    HDC hdc = lvcd->nmcd.hdc;
    HDC hdcSource = CreateCompatibleDC(hdc);
    unsigned row = lvcd->nmcd.dwItemSpec;
    for(auto column : range(state().columns)) {
      RECT rc, rcLabel;
      ListView_GetSubItemRect(hwnd, row, column, LVIR_BOUNDS, &rc);
      ListView_GetSubItemRect(hwnd, row, column, LVIR_LABEL, &rcLabel);
      rc.right = rcLabel.right;  //bounds of column 0 returns width of entire item
      signed iconSize = rc.bottom - rc.top - 1;
      bool checked = state().items(row)->state.checked;
      bool selected = state().items(row)->state.selected;
      HBRUSH brush = CreateSolidBrush(selected ? GetSysColor(COLOR_HIGHLIGHT) : CreateRGB(_backgroundColor(row, column)));
      FillRect(hdc, &rc, brush);
      DeleteObject(brush);
      if(state().checkable && column == 0) {
        if(auto htheme = OpenThemeData(hwnd, L"BUTTON")) {
          unsigned state = checked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL;
          SIZE size;
          GetThemePartSize(htheme, hdc, BP_CHECKBOX, state, NULL, TS_TRUE, &size);
          signed center = max(0, (rc.bottom - rc.top - size.cy) / 2);
          RECT rd{rc.left + center, rc.top + center, rc.left + center + size.cx, rc.top + center + size.cy};
          DrawThemeBackground(htheme, hdc, BP_CHECKBOX, state, &rd, NULL);
          CloseThemeData(htheme);
        }
        rc.left += iconSize + 2;
      } else {
        rc.left += 2;
      }
      auto cell = self().item(row)->cell(column);
      if(!cell) continue;
      if(auto icon = cell->state.icon) {
        icon.scale(iconSize, iconSize);
        icon.transform();
        auto bitmap = CreateBitmap(icon);
        SelectBitmap(hdcSource, bitmap);
        BLENDFUNCTION blend{AC_SRC_OVER, 0, (BYTE)(selected ? 128 : 255), AC_SRC_ALPHA};
        AlphaBlend(hdc, rc.left, rc.top, iconSize, iconSize, hdcSource, 0, 0, iconSize, iconSize, blend);
        DeleteObject(bitmap);
        rc.left += iconSize + 2;
      }
      if(auto text = cell->state.text) {
        auto halign = state().columns(column)->state.horizontalAlignment;
        auto valign = state().columns(column)->state.verticalAlignment;
        utf16_t wText(text);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, selected ? GetSysColor(COLOR_HIGHLIGHTTEXT) : CreateRGB(_foregroundColor(row, column)));
        auto style = DT_SINGLELINE | DT_END_ELLIPSIS;
        style |= halign < 0.333 ? DT_LEFT : halign > 0.666 ? DT_RIGHT : DT_CENTER;
        style |= valign < 0.333 ? DT_TOP : valign > 0.666 ? DT_BOTTOM : DT_VCENTER;
        rc.right -= 2;
        auto font = pFont::create(_font(row, column));
        SelectObject(hdc, font);
        DrawText(hdc, wText, -1, &rc, style);
        DeleteObject(font);
      }
      if(state().gridVisible) {
        ListView_GetSubItemRect(hwnd, row, column, LVIR_BOUNDS, &rc);
        rc.top = rc.bottom - 1;
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
        ListView_GetSubItemRect(hwnd, row, column, LVIR_LABEL, &rc);
        rc.left = rc.right - 1;
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
      }
    }
    DeleteDC(hdcSource);
    return CDRF_SKIPDEFAULT;
  }
  }
}

auto pListView::onSort(LPARAM lparam) -> void {
  auto nmlistview = (LPNMLISTVIEW)lparam;
  self().doSort(self().column(nmlistview->iSubItem));
}

auto pListView::_backgroundColor(unsigned _row, unsigned _column) -> Color {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto color = cell->backgroundColor()) return color;
    }
    if(auto color = item->backgroundColor()) return color;
  }
  if(auto column = self().column(_column)) {
    if(auto color = column->backgroundColor()) return color;
  }
  if(auto color = self().backgroundColor()) return color;
  if(state().columns.size() >= 2 && _row % 2) return {240, 240, 240};
  return {255, 255, 255};
}

auto pListView::_cellWidth(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 6;
  if(state().checkable && _column == 0) width += 16 + 2;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto& icon = cell->state.icon) {
        width += 16 + 2;
      }
      if(auto& text = cell->state.text) {
        width += Font::size(_font(_row, _column), text).width();
      }
    }
  }
  return width;
}

auto pListView::_columnWidth(unsigned _column) -> unsigned {
  unsigned width = 12;
  if(auto column = self().column(_column)) {
    if(auto& icon = column->state.icon) {
      width += 16 + 12;  //yes; icon spacing in column headers is excessive
    }
    if(auto& text = column->state.text) {
      width += Font::size(self().font(true), text).width();
    }
  }
  return width;
}

auto pListView::_font(unsigned _row, unsigned _column) -> string {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto font = cell->font()) return font;
    }
    if(auto font = item->font()) return font;
  }
  if(auto column = self().column(_column)) {
    if(auto font = column->font()) return font;
  }
  if(auto font = self().font(true)) return font;
  return Font::sans(8);
}

auto pListView::_foregroundColor(unsigned _row, unsigned _column) -> Color {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto color = cell->foregroundColor()) return color;
    }
    if(auto color = item->foregroundColor()) return color;
  }
  if(auto column = self().column(_column)) {
    if(auto color = column->foregroundColor()) return color;
  }
  if(auto color = self().foregroundColor()) return color;
  return {0, 0, 0};
}

auto pListView::_setIcons() -> void {
  ListView_SetImageList(hwnd, NULL, LVSIL_SMALL);
  if(imageList) ImageList_Destroy(imageList);
  imageList = ImageList_Create(16, 16, ILC_COLOR32, 1, 0);
  ListView_SetImageList(hwnd, imageList, LVSIL_SMALL);

  for(auto column : range(state().columns)) {
    auto icon = state().columns(column)->state.icon;
    if(icon) {
      icon.scale(16, 16);
      icon.transform();
    } else {
      icon.allocate(16, 16);
      icon.fill(0x00ffffff);
    }
    auto bitmap = CreateBitmap(icon);
    ImageList_Add(imageList, bitmap, NULL);
    DeleteObject(bitmap);
  }

  //empty icon used for ListViewItems (drawn manually via onCustomDraw)
  image icon;
  icon.allocate(16, 16);
  icon.fill(0x00ffffff);
  auto bitmap = CreateBitmap(icon);
  ImageList_Add(imageList, bitmap, NULL);
  DeleteObject(bitmap);
}

auto pListView::_width(unsigned column) -> unsigned {
  if(auto width = state().columns[column]->width()) return width;
  unsigned width = 1;
  if(state().headerVisible) {
    width = max(width, _columnWidth(column));
  }
  for(auto row : range(state().items)) {
    width = max(width, _cellWidth(row, column));
  }
  return width;
}

}

#endif
