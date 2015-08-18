#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
}

auto pListViewItem::destruct() -> void {
  if(qtItem) {
    delete qtItem;
    qtItem = nullptr;
  }
}

auto pListViewItem::append(sListViewCell cell) -> void {
}

auto pListViewItem::remove(sListViewCell cell) -> void {
}

auto pListViewItem::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pListViewItem::setFont(const string& font) -> void {
  _setState();
}

auto pListViewItem::setForegroundColor(Color color) -> void {
  _setState();
}

auto pListViewItem::setSelected(bool selected) -> void {
  _setState();
}

auto pListViewItem::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pListViewItem::_setState() -> void {
  if(auto parent = _parent()) {
    qtItem->setSelected(state().selected);
    if(state().selected) {
      parent->qtListView->setCurrentItem(qtItem);
    }
    for(auto& cell : state().cells) {
      if(auto self = cell->self()) self->_setState();
    }
  }
}

}

#endif
