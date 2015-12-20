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
    WS_EX_CLIENTEDGE | LVS_EX_DOUBLEBUFFER, WC_LISTVIEW, L"",
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
  setBordered(state().bordered);
  _setIcons();
  _setSortable();
  resizeColumns();
}

auto pListView::destruct() -> void {
  if(imageList) { ImageList_Destroy(imageList); imageList = nullptr; }
  DestroyWindow(hwnd);
}

auto pListView::append(sListViewHeader header) -> void {
}

auto pListView::append(sListViewItem item) -> void {
}

auto pListView::remove(sListViewHeader header) -> void {
  LVCOLUMN lvColumn{LVCF_WIDTH};
  while(ListView_GetColumn(hwnd, 0, &lvColumn)) {
    ListView_DeleteColumn(hwnd, 0);
  }
}

auto pListView::remove(sListViewItem item) -> void {
}

auto pListView::resizeColumns() -> void {
  lock();

  if(auto& header = state().header) {
    vector<signed> widths;
    signed minimumWidth = 0;
    signed expandable = 0;
    for(auto column : range(header->columnCount())) {
      signed width = _width(column);
      widths.append(width);
      minimumWidth += width;
      if(header->column(column).expandable()) expandable++;
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

    for(auto column : range(header->columnCount())) {
      if(auto self = header->state.columns[column]->self()) {
        signed width = widths[column];
        if(self->state().expandable) width += expandWidth;
        self->_width = width;
        self->_setState();
      }
    }
  }

  unlock();
}

auto pListView::setAlignment(Alignment alignment) -> void {
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

auto pListView::setBordered(bool bordered) -> void {
  //rendered via onCustomDraw
}

auto pListView::setForegroundColor(Color color) -> void {
}

auto pListView::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  if(auto& header = state().header) {
    for(auto& column : header->state.columns) {
      if(column->state.expandable) return resizeColumns();
    }
  }
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
    auto& header = state().header;
    if(!header) break;
    for(auto column : range(header->columnCount())) {
      RECT rc, rcLabel;
      ListView_GetSubItemRect(hwnd, row, column, LVIR_BOUNDS, &rc);
      ListView_GetSubItemRect(hwnd, row, column, LVIR_LABEL, &rcLabel);
      rc.right = rcLabel.right;  //bounds of column 0 returns width of entire item
      signed iconSize = rc.bottom - rc.top - 1;
      bool selected = state().items(row)->state.selected;

      if(auto cell = self().item(row)->cell(column)) {
        auto backgroundColor = cell->backgroundColor(true);
        HBRUSH brush = CreateSolidBrush(
          selected ? GetSysColor(COLOR_HIGHLIGHT)
        : backgroundColor ? CreateRGB(backgroundColor)
        : GetSysColor(COLOR_WINDOW)
        );
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);

        if(cell->state.checkable) {
          if(auto htheme = OpenThemeData(hwnd, L"BUTTON")) {
            unsigned state = cell->state.checked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL;
            SIZE size;
            GetThemePartSize(htheme, hdc, BP_CHECKBOX, state, nullptr, TS_TRUE, &size);
            signed center = max(0, (rc.bottom - rc.top - size.cy) / 2);
            RECT rd{rc.left + center, rc.top + center, rc.left + center + size.cx, rc.top + center + size.cy};
            DrawThemeBackground(htheme, hdc, BP_CHECKBOX, state, &rd, nullptr);
            CloseThemeData(htheme);
          } else {
            //Windows Classic
            rc.left += 2;
            RECT rd{rc.left, rc.top, rc.left + iconSize, rc.top + iconSize};
            DrawFrameControl(hdc, &rd, DFC_BUTTON, DFCS_BUTTONCHECK | (cell->state.checked ? DFCS_CHECKED : 0));
          }
          rc.left += iconSize + 2;
        } else {
          rc.left += 2;
        }

        if(auto& image = cell->state.image) {
          auto bitmap = CreateBitmap(image);
          SelectBitmap(hdcSource, bitmap);
          BLENDFUNCTION blend{AC_SRC_OVER, 0, (BYTE)(selected ? 128 : 255), AC_SRC_ALPHA};
          AlphaBlend(hdc, rc.left, rc.top, iconSize, iconSize, hdcSource, 0, 0, image.width(), image.height(), blend);
          DeleteObject(bitmap);
          rc.left += iconSize + 2;
        }

        if(auto text = cell->state.text) {
          auto alignment = cell->alignment(true);
          if(!alignment) alignment = {0.0, 0.5};
          utf16_t wText(text);
          SetBkMode(hdc, TRANSPARENT);
          auto foregroundColor = cell->foregroundColor(true);
          SetTextColor(hdc,
            selected ? GetSysColor(COLOR_HIGHLIGHTTEXT)
          : foregroundColor ? CreateRGB(foregroundColor)
          : GetSysColor(COLOR_WINDOWTEXT)
          );
          auto style = DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;
          style |= alignment.horizontal() < 0.333 ? DT_LEFT : alignment.horizontal() > 0.666 ? DT_RIGHT  : DT_CENTER;
          style |= alignment.vertical()   < 0.333 ? DT_TOP  : alignment.vertical()   > 0.666 ? DT_BOTTOM : DT_VCENTER;
          rc.right -= 2;
          auto font = pFont::create(cell->font(true));
          SelectObject(hdc, font);
          DrawText(hdc, wText, -1, &rc, style);
          DeleteObject(font);
        }
      } else {
        auto backgroundColor = state().backgroundColor;
        HBRUSH brush = CreateSolidBrush(
          selected ? GetSysColor(COLOR_HIGHLIGHT)
        : backgroundColor ? CreateRGB(backgroundColor)
        : GetSysColor(COLOR_WINDOW)
        );
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
      }

      if(state().bordered) {
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

  return CDRF_SKIPDEFAULT;
}

auto pListView::onSort(LPARAM lparam) -> void {
  auto nmlistview = (LPNMLISTVIEW)lparam;
  if(auto& header = state().header) {
    if(auto column = header->column(nmlistview->iSubItem)) {
      if(column->sortable()) self().doSort(column);
    }
  }
}

auto pListView::onToggle(LPARAM lparam) -> void {
  auto itemActivate = (LPNMITEMACTIVATE)lparam;
  LVHITTESTINFO hitTestInfo{0};
  hitTestInfo.pt = itemActivate->ptAction;
  ListView_SubItemHitTest(hwnd, &hitTestInfo);

  if(auto cell = self().item(hitTestInfo.iItem).cell(hitTestInfo.iSubItem)) {
    if(cell->state.checkable) {
      cell->state.checked = !cell->state.checked;
      if(!locked()) self().doToggle(cell);
      //todo: try to find a way to only repaint this cell instead of the entire control to reduce flickering
      PostMessageOnce(_parentHandle(), AppMessage::ListView_doPaint, 0, (LPARAM)&reference);
    }
  }
}

auto pListView::_backgroundColor(unsigned _row, unsigned _column) -> Color {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto color = cell->backgroundColor()) return color;
    }
    if(auto color = item->backgroundColor()) return color;
  }
//  if(auto column = self().column(_column)) {
//    if(auto color = column->backgroundColor()) return color;
//  }
  if(auto color = self().backgroundColor()) return color;
//  if(state().columns.size() >= 2 && _row % 2) return {240, 240, 240};
  return {255, 255, 255};
}

auto pListView::_cellWidth(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 6;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(cell->state.checkable) {
        width += 16 + 2;
      }
      if(auto& image = cell->state.image) {
        width += 16 + 2;
      }
      if(auto& text = cell->state.text) {
        width += pFont::size(_font(_row, _column), text).width();
      }
    }
  }
  return width;
}

auto pListView::_columnWidth(unsigned _column) -> unsigned {
  unsigned width = 12;
  if(auto header = state().header) {
    if(auto column = header->column(_column)) {
      if(auto& image = column->state.image) {
        width += 16 + 12;  //yes; icon spacing in column headers is excessive
      }
      if(auto& text = column->state.text) {
        width += pFont::size(self().font(true), text).width();
      }
    }
  }
  return width;
}

auto pListView::_font(unsigned _row, unsigned _column) -> Font {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto font = cell->font()) return font;
    }
    if(auto font = item->font()) return font;
  }
//  if(auto column = self().column(_column)) {
//    if(auto font = column->font()) return font;
//  }
  if(auto font = self().font(true)) return font;
  return {};
}

auto pListView::_foregroundColor(unsigned _row, unsigned _column) -> Color {
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto color = cell->foregroundColor()) return color;
    }
    if(auto color = item->foregroundColor()) return color;
  }
//  if(auto column = self().column(_column)) {
//    if(auto color = column->foregroundColor()) return color;
//  }
  if(auto color = self().foregroundColor()) return color;
  return {0, 0, 0};
}

auto pListView::_setIcons() -> void {
  ListView_SetImageList(hwnd, nullptr, LVSIL_SMALL);
  if(imageList) ImageList_Destroy(imageList);
  imageList = ImageList_Create(16, 16, ILC_COLOR32, 1, 0);
  ListView_SetImageList(hwnd, imageList, LVSIL_SMALL);

  if(auto& header = state().header) {
    for(auto column : range(header->columnCount())) {
      nall::image icon;
      if(auto& image = header->state.columns[column]->state.image) {
        icon.allocate(image.width(), image.height());
        memory::copy(icon.data(), image.data(), icon.size());
        icon.scale(16, 16);
      } else {
        icon.allocate(16, 16);
        icon.fill(0x00ffffff);
      }
      auto bitmap = CreateBitmap(icon);
      ImageList_Add(imageList, bitmap, nullptr);
      DeleteObject(bitmap);
    }
  }

  //empty icon used for ListViewItems (drawn manually via onCustomDraw)
  nall::image icon;
  icon.allocate(16, 16);
  icon.fill(0x00ffffff);
  auto bitmap = CreateBitmap(icon);
  ImageList_Add(imageList, bitmap, nullptr);
  DeleteObject(bitmap);
}

auto pListView::_setSortable() -> void {
  bool sortable = false;
  if(auto& header = state().header) {
    for(auto& column : header->state.columns) {
      if(column->sortable()) sortable = true;
    }
  }

  //note: this won't change the visual style: WC_LISTVIEW caches this in CreateWindow
  auto style = GetWindowLong(hwnd, GWL_STYLE);
  !sortable ? style |= LVS_NOSORTHEADER : style &=~ LVS_NOSORTHEADER;
  SetWindowLong(hwnd, GWL_STYLE, style);
}

auto pListView::_width(unsigned column) -> unsigned {
  if(auto& header = state().header) {
    if(auto width = header->state.columns[column]->width()) return width;
    unsigned width = 1;
    if(header->visible()) width = max(width, _columnWidth(column));
    for(auto row : range(state().items)) {
      width = max(width, _cellWidth(row, column));
    }
    return width;
  }
  return 1;
}

}

#endif
