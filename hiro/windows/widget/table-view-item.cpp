#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewItem::construct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    wchar_t text[] = L"";
    LVITEM lvItem{0};
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = self().offset();
    lvItem.iSubItem = 0;
    lvItem.pszText = text;
    ListView_InsertItem(parent->hwnd, &lvItem);
    _setState();
    parent->unlock();
  }
}

auto pTableViewItem::destruct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_DeleteItem(parent->hwnd, self().offset());
    parent->unlock();
  }
}

auto pTableViewItem::append(sTableViewCell cell) -> void {
}

auto pTableViewItem::remove(sTableViewCell cell) -> void {
}

auto pTableViewItem::setAlignment(Alignment alignment) -> void {
}

auto pTableViewItem::setBackgroundColor(Color color) -> void {
}

auto pTableViewItem::setFocused() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_SetItemState(parent->hwnd, self().offset(), LVIS_FOCUSED, LVIS_FOCUSED);
    parent->unlock();
  }
}

auto pTableViewItem::setForegroundColor(Color color) -> void {
}

auto pTableViewItem::setSelected(bool selected) -> void {
  _setState();
}

auto pTableViewItem::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewItem::_setState() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_SetItemState(parent->hwnd, self().offset(), state().selected ? LVIS_SELECTED : 0, LVIS_SELECTED);
    for(auto& cell : state().cells) {
      if(auto self = cell->self()) self->_setState();
    }
    parent->unlock();
  }
}

}

#endif
