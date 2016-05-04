#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewCell::construct() -> void {
}

auto pTableViewCell::destruct() -> void {
}

auto pTableViewCell::setAlignment(Alignment alignment) -> void {
}

auto pTableViewCell::setBackgroundColor(Color color) -> void {
  _repaint();
}

auto pTableViewCell::setCheckable(bool checkable) -> void {
  _repaint();
}

auto pTableViewCell::setChecked(bool checked) -> void {
  _repaint();
}

auto pTableViewCell::setForegroundColor(Color color) -> void {
  _repaint();
}

auto pTableViewCell::setIcon(const image& icon) -> void {
  _repaint();
}

auto pTableViewCell::setText(const string& text) -> void {
  _repaint();
}

auto pTableViewCell::_parent() -> maybe<pTableViewItem&> {
  if(auto parent = self().parentTableViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewCell::_repaint() -> void {
  if(auto parent = _parent()) {
    if(auto tableView = parent->_parent()) {
      //TableView uses a custom drawing routine; so we need to tell the control to repaint itself manually
      PostMessageOnce(tableView->_parentHandle(), AppMessage::TableView_doPaint, 0, (LPARAM)&tableView->reference);
    }
  }
}

auto pTableViewCell::_setState() -> void {
  if(auto item = _parent()) {
    if(auto parent = item->_parent()) {
      parent->lock();
      wchar_t text[] = L"";
      LVITEM lvItem;
      lvItem.mask = LVIF_TEXT;
      lvItem.iItem = item->self().offset();
      lvItem.iSubItem = self().offset();
      lvItem.pszText = text;
      ListView_SetItem(parent->hwnd, &lvItem);
      parent->unlock();
    }
  }
}

}

#endif
