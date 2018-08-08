#if defined(Hiro_TabFrame)

namespace hiro {

static auto CALLBACK TabFrame_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto tabFrame = (mTabFrame*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto self = tabFrame->self()) {
      return Shared_windowProc(self->defaultWindowProc, hwnd, msg, wparam, lparam);
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pTabFrame::construct() -> void {
  hwnd = CreateWindow(
    WC_TABCONTROL, L"", WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)TabFrame_windowProc);
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
  _synchronizeSizable();
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
  TabCtrl_DeleteItem(hwnd, item->offset());
  _buildImageList();
}

auto pTabFrame::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  for(auto& item : state().items) {
    if(auto& sizable = item->state.sizable) {
      if(auto self = sizable->self()) self->setEnabled(sizable->enabled());
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
    if(auto& sizable = item->state.sizable) {
      sizable->setGeometry(geometry);
    }
  }
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
  //unsupported
}

auto pTabFrame::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  for(auto& item : state().items) {
    if(auto& sizable = item->state.sizable) {
      if(auto self = sizable->self()) self->setVisible(sizable->visible(true));
    }
  }
}

//

auto pTabFrame::_buildImageList() -> void {
  unsigned size = pFont::size(hfont, " ").height();

  if(imageList) { ImageList_Destroy(imageList); imageList = nullptr; }
  imageList = ImageList_Create(size, size, ILC_COLOR32, 1, 0);
  for(auto& item : state().items) {
    ImageList_Append(imageList, item->state.icon, size);
  }
  TabCtrl_SetImageList(hwnd, imageList);
  for(auto offset : range(state().items.size())) {
    TCITEM tcItem;
    tcItem.mask = TCIF_IMAGE;
    tcItem.iImage = state().items[offset]->state.icon ? offset : -1;
    TabCtrl_SetItem(hwnd, offset, &tcItem);
  }
}

//hide all TabFrameItems, and then display the selected TabFrameItem
auto pTabFrame::_synchronizeSizable() -> void {
  for(auto& item : state().items) {
    if(auto& sizable = item->state.sizable) sizable->setVisible(false);
  }
  //without this call, widgets from the previous tab will remain visible
  //alongside widgets from the newly selected tab for about one frame ...
  Application::processEvents();
  uint selected = TabCtrl_GetCurSel(hwnd);
  if(auto item = self().item(selected)) {
    if(auto& sizable = item->state.sizable) sizable->setVisible(true);
  }
}

auto pTabFrame::onChange() -> void {
  uint selected = TabCtrl_GetCurSel(hwnd);
  for(auto& item : state().items) item->state.selected = false;
  if(auto item = self().item(selected)) item->state.selected = true;
  _synchronizeSizable();
  self().doChange();
}

}

#endif
