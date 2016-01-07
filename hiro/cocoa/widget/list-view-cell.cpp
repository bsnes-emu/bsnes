#if defined(Hiro_ListView)

namespace hiro {

auto pListViewCell::construct() -> void {
}

auto pListViewCell::destruct() -> void {
}

auto pListViewCell::setAlignment(Alignment alignment) -> void {
}

auto pListViewCell::setBackgroundColor(Color color) -> void {
}

auto pListViewCell::setCheckable(bool checkable) -> void {
}

auto pListViewCell::setChecked(bool checked) -> void {
}

auto pListViewCell::setForegroundColor(Color color) -> void {
}

auto pListViewCell::setIcon(const image& icon) -> void {
}

auto pListViewCell::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto pListView = _grandparent()) {
      [[pListView->cocoaView content] reloadData];
    }
  }
}

auto pListViewCell::_grandparent() -> maybe<pListView&> {
  if(auto parent = _parent()) return parent->_parent();
}

auto pListViewCell::_parent() -> maybe<pListViewItem&> {
  if(auto parent = self().parentListViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
