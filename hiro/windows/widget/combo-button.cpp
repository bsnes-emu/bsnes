#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButton::construct() -> void {
  hwnd = CreateWindow(
    L"COMBOBOX", L"",
    WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, 0, 0,
    _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  for(auto& item : state().items) append(item);
}

auto pComboButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pComboButton::append(sComboButtonItem item) -> void {
  lock();
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)(wchar_t*)utf16_t(item->state.text));
  if(item->state.selected) SendMessage(hwnd, CB_SETCURSEL, item->offset(), 0);
  if(SendMessage(hwnd, CB_GETCURSEL, 0, 0) == CB_ERR) item->setSelected();
  unlock();
}

auto pComboButton::minimumSize() const -> Size {
  signed width = 0;
  for(auto& item : state().items) {
    width = max(width, pFont::size(hfont, item->state.text).width());
  }
  return {width + 24, pFont::size(hfont, " ").height() + 10};
}

auto pComboButton::remove(sComboButtonItem item) -> void {
  lock();
  SendMessage(hwnd, CB_DELETESTRING, item->offset(), 0);
  if(item->state.selected) SendMessage(hwnd, CB_SETCURSEL, 0, 0);
  unlock();
}

auto pComboButton::reset() -> void {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

auto pComboButton::setGeometry(Geometry geometry) -> void {
  //height = minimum drop-down list height; use CB_SETITEMHEIGHT to control actual widget height
  pWidget::setGeometry({geometry.x(), geometry.y(), geometry.width(), 1});
  RECT rc;
  GetWindowRect(hwnd, &rc);
  unsigned adjustedHeight = geometry.height() - ((rc.bottom - rc.top) - SendMessage(hwnd, CB_GETITEMHEIGHT, (WPARAM)-1, 0));
  SendMessage(hwnd, CB_SETITEMHEIGHT, (WPARAM)-1, adjustedHeight);
}

auto pComboButton::onChange() -> void {
  signed offset = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
  if(offset == CB_ERR) return;
  for(auto& item : state().items) item->state.selected = false;
  if(auto item = self().item(offset)) item->setSelected();
  self().doChange();
}

}

#endif
