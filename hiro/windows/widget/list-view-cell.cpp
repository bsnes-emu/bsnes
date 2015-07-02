#if defined(Hiro_ListView)

namespace hiro {

auto pListViewCell::construct() -> void {
}

auto pListViewCell::destruct() -> void {
}

auto pListViewCell::setBackgroundColor(Color color) -> void {
}

auto pListViewCell::setForegroundColor(Color color) -> void {
}

auto pListViewCell::setIcon(const image& icon) -> void {
}

auto pListViewCell::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    if(auto listView = parent->_parent()) {
      //ListView uses a custom drawing routine; so we need to tell the control to repaint itself manually
      PostMessageOnce(listView->_parentHandle(), AppMessage::ListView_doPaint, 0, (LPARAM)&listView->reference);
    }
  }
}

auto pListViewCell::_parent() -> maybe<pListViewItem&> {
  if(auto parent = self().parentListViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewCell::_setState() -> void {
  if(auto item = _parent()) {
    if(auto parent = item->_parent()) {
      parent->lock();
      wchar_t text[] = L"";
      LVITEM lvItem;
      lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
      lvItem.iItem = item->self().offset();
      lvItem.iSubItem = self().offset();
      lvItem.iImage = parent->self().columns();
      lvItem.pszText = text;
      ListView_SetItem(parent->hwnd, &lvItem);
      parent->unlock();
    }
  }
}

}

#endif
