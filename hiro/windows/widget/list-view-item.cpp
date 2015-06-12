#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
}

auto pListViewItem::destruct() -> void {
}

auto pListViewItem::append(sListViewCell cell) -> void {
}

auto pListViewItem::remove(sListViewCell cell) -> void {
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
}

auto pListViewItem::setChecked(bool checked) -> void {
  if(auto parent = _parent()) {
    parent->lock();
    ListView_SetCheckState(parent->hwnd, self().offset(), checked);
    parent->unlock();
  }
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
  if(auto parent = _parent()) {
    parent->lock();
    unsigned state = selected ? LVIS_SELECTED : 0;
    ListView_SetItemState(parent->hwnd, self().offset(), state, LVIS_SELECTED);
    parent->unlock();
  }
}

auto pListViewItem::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
