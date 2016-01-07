#if defined(Hiro_TabFrame)

namespace hiro {

static auto CALLBACK TabFrame_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto tabFrame = dynamic_cast<mTabFrame*>(object)) {
      if(auto self = tabFrame->self()) {
        return Shared_windowProc(self->windowProc, hwnd, msg, wparam, lparam);
      }
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pTabFrame::construct() -> void {
  hwnd = CreateWindow(
    WC_TABCONTROL, L"", WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)TabFrame_windowProc);
  pWidget::_setState();
  for(auto& item : state().items) append(item);
}

auto pTabFrame::destruct() -> void {
  if(imageList) { ImageList_Destroy(imageList); imageList = nullptr; }
  DestroyWindow(hwnd);
}

auto pTabFrame::append(sTabFrameItem item) -> void {
  wchar_t text[] = L"";
  TCITEM tcItem;
  tcItem.mask = TCIF_TEXT;
  tcItem.pszText = text;
  TabCtrl_InsertItem(hwnd, item->offset(), &tcItem);
  if(auto self = item->self()) {
    self->setClosable(item->state.closable);
    self->setIcon(item->state.icon);
    self->setMovable(item->state.movable);
    self->setText(item->state.text);
    if(item->selected()) self->setSelected();
  }
  _buildImageList();
  _synchronizeLayout();
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
  TabCtrl_DeleteItem(hwnd, item->offset());
  _buildImageList();
}

auto pTabFrame::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  for(auto& item : state().items) {
    if(auto layout = item->state.layout) {
      if(auto self = layout->self()) self->setEnabled(layout->enabled(true));
    }
  }
}

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  geometry.setX(geometry.x() + 1);
  geometry.setY(geometry.y() + 21);
  geometry.setWidth(geometry.width() - 4);
  geometry.setHeight(geometry.height() - 23);
  for(auto& item : state().items) {
    if(auto layout = item->state.layout) {
      layout->setGeometry(geometry);
    }
  }
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
  //unsupported
}

auto pTabFrame::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  for(auto& item : state().items) {
    if(auto layout = item->state.layout) {
      if(auto self = layout->self()) self->setVisible(layout->visible(true));
    }
  }
}

auto pTabFrame::_buildImageList() -> void {
  unsigned size = pFont::size(hfont, " ").height();

  if(imageList) { ImageList_Destroy(imageList); imageList = nullptr; }
  imageList = ImageList_Create(size, size, ILC_COLOR32, 1, 0);
  for(auto& item : state().items) {
    ImageList_Append(imageList, item->state.icon, size);
  }
  TabCtrl_SetImageList(hwnd, imageList);
  for(auto offset : range(state().items)) {
    TCITEM tcItem;
    tcItem.mask = TCIF_IMAGE;
    tcItem.iImage = state().items[offset]->state.icon ? offset : -1;
    TabCtrl_SetItem(hwnd, offset, &tcItem);
  }
}

auto pTabFrame::_synchronizeLayout() -> void {
  for(auto& item : state().items) {
    if(auto layout = item->state.layout) {
      layout->setVisible(item->selected());
    }
  }
}

auto pTabFrame::onChange() -> void {
  unsigned selected = TabCtrl_GetCurSel(hwnd);
  for(auto& item : state().items) item->state.selected = false;
  if(auto item = self().item(selected)) item->state.selected = true;
  _synchronizeLayout();
  self().doChange();
}

//called only if TCS_OWNERDRAWFIXED style is used
//this style disables XP/Vista theming of the TabFrame
auto pTabFrame::onDrawItem(LPARAM lparam) -> void {
/*
  auto item = (LPDRAWITEMSTRUCT)lparam;
  FillRect(item->hDC, &item->rcItem, GetSysColorBrush(COLOR_3DFACE));
  SetBkMode(item->hDC, TRANSPARENT);
  SetTextColor(item->hDC, GetSysColor(COLOR_BTNTEXT));

  unsigned selection = item->itemID;
  if(selection < tabFrame.state.text.size()) {
    string text = tabFrame.state.text[selection];
    Size size = pFont::size(hfont, text);
    unsigned width = item->rcItem.right - item->rcItem.left + 1;
    if(!tabFrame.state.image[selection].empty()) {
      width += size.height + 2;
      ImageList_Draw(imageList, selection, item->hDC, item->rcItem.left + (width - size.width) / 2 - (size.height + 3), item->rcItem.top + 2, ILD_NORMAL);
    }
    TextOut(item->hDC, item->rcItem.left + (width - size.width) / 2, item->rcItem.top + 2, utf16_t(text), text.size());
  }
*/
}

}

#endif
