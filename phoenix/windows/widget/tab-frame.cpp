namespace phoenix {

static LRESULT CALLBACK TabFrame_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object* object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  TabFrame& tabFrame = (TabFrame&)*object;
  return Shared_windowProc(tabFrame.p.windowProc, hwnd, msg, wparam, lparam);
}

void pTabFrame::append(string text, const image& image) {
  unsigned selection = TabCtrl_GetItemCount(hwnd);
  wchar_t wtext[] = L"";
  TCITEM item;
  item.mask = TCIF_TEXT;
  item.pszText = wtext;
  TabCtrl_InsertItem(hwnd, selection, &item);
  setText(selection, text);
  if(!image.empty()) setImage(selection, image);
}

void pTabFrame::remove(unsigned selection) {
  TabCtrl_DeleteItem(hwnd, selection);
  buildImageList();
}

void pTabFrame::setEnabled(bool enabled) {
  pWidget::setEnabled(enabled);
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  geometry.x += 1, geometry.width -= 4;
  geometry.y += 21, geometry.height -= 23;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setGeometry(geometry);
  }
}

void pTabFrame::setImage(unsigned selection, const image& image) {
  buildImageList();
}

void pTabFrame::setSelection(unsigned selection) {
  TabCtrl_SetCurSel(hwnd, selection);
  synchronizeLayout();
}

void pTabFrame::setText(unsigned selection, string text) {
  utf16_t wtext(text);
  TCITEM item;
  item.mask = TCIF_TEXT;
  item.pszText = (wchar_t*)wtext;
  TabCtrl_SetItem(hwnd, selection, &item);
}

void pTabFrame::setVisible(bool visible) {
  pWidget::setVisible(visible);
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
}

void pTabFrame::constructor() {
  hwnd = CreateWindow(WC_TABCONTROL, L"",
    WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&tabFrame);

  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)TabFrame_windowProc);

  setDefaultFont();
  for(auto& text : tabFrame.state.text) append(text, {});
  buildImageList();
  setSelection(tabFrame.state.selection);
  synchronize();
}

void pTabFrame::destructor() {
  if(imageList) { ImageList_Destroy(imageList); imageList = nullptr; }
  DestroyWindow(hwnd);
}

void pTabFrame::orphan() {
  destructor();
  constructor();
}

void pTabFrame::buildImageList() {
  if(imageList) ImageList_Destroy(imageList);
  unsigned size = pFont::size(hfont, " ").height;
  imageList = ImageList_Create(size, size, ILC_COLOR32, 1, 0);
  for(auto& image : tabFrame.state.image) {
    ImageList_Append(imageList, image);
  }
  TabCtrl_SetImageList(hwnd, imageList);
  for(unsigned n = 0; n < tabFrame.state.image.size(); n++) {
    TCITEM item;
    item.mask = TCIF_IMAGE;
    item.iImage = (tabFrame.state.image(n).empty() ? -1 : n);
    TabCtrl_SetItem(hwnd, n, &item);
  }
}

void pTabFrame::synchronizeLayout() {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}

void pTabFrame::onChange() {
  tabFrame.state.selection = TabCtrl_GetCurSel(hwnd);
  synchronizeLayout();
  if(tabFrame.onChange) tabFrame.onChange();
}

//called only if TCS_OWNERDRAWFIXED style is used
//this style disables XP/Vista theming of the TabFrame
void pTabFrame::onDrawItem(LPARAM lparam) {
  LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lparam;
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
}

}
