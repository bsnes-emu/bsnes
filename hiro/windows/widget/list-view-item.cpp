#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
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

auto pListViewItem::destruct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_DeleteItem(parent->hwnd, self().offset());
    parent->unlock();
  }
}

auto pListViewItem::append(sListViewCell cell) -> void {
}

auto pListViewItem::remove(sListViewCell cell) -> void {
}

auto pListViewItem::setAlignment(Alignment alignment) -> void {
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
}

auto pListViewItem::setFocused() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_SetItemState(parent->hwnd, self().offset(), LVIS_FOCUSED, LVIS_FOCUSED);
    parent->unlock();
  }
}

auto pListViewItem::setForegroundColor(Color color) -> void {
}

auto pListViewItem::setSelected(bool selected) -> void {
  _setState();
}

auto pListViewItem::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewItem::_setState() -> void {
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
