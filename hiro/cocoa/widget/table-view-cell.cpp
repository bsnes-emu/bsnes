#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewCell::construct() -> void {
}

auto pTableViewCell::destruct() -> void {
}

auto pTableViewCell::setAlignment(Alignment alignment) -> void {
}

auto pTableViewCell::setBackgroundColor(Color color) -> void {
}

auto pTableViewCell::setCheckable(bool checkable) -> void {
}

auto pTableViewCell::setChecked(bool checked) -> void {
}

auto pTableViewCell::setForegroundColor(Color color) -> void {
}

auto pTableViewCell::setIcon(const image& icon) -> void {
}

auto pTableViewCell::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto pTableView = _grandparent()) {
      [[pTableView->cocoaView content] reloadData];
    }
  }
}

auto pTableViewCell::_grandparent() -> maybe<pTableView&> {
  if(auto parent = _parent()) return parent->_parent();
}

auto pTableViewCell::_parent() -> maybe<pTableViewItem&> {
  if(auto parent = self().parentTableViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
