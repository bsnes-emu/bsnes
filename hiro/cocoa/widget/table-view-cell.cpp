#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewCell::construct() -> void {
}

auto pTableViewCell::destruct() -> void {
}

auto pTableViewCell::setAlignment(Alignment alignment) -> void {
  _redraw();
}

auto pTableViewCell::setBackgroundColor(Color color) -> void {
  _redraw();
}

auto pTableViewCell::setCheckable(bool checkable) -> void {
  _redraw();
}

auto pTableViewCell::setChecked(bool checked) -> void {
  _redraw();
}

auto pTableViewCell::setForegroundColor(Color color) -> void {
  _redraw();
}

auto pTableViewCell::setIcon(const image& icon) -> void {
  _redraw();
}

auto pTableViewCell::setText(const string& text) -> void {
  _redraw();
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

auto pTableViewCell::_redraw() -> void {
  @autoreleasepool {
    if(auto pTableViewItem = _parent()) {
      if(auto pTableView = _grandparent()) {
        auto column = self().offset();
        auto row = pTableViewItem->self().offset();
        NSRect rect = [[pTableView->cocoaTableView content] frameOfCellAtColumn:column row:row];
        [[pTableView->cocoaTableView content] setNeedsDisplayInRect:rect];
      }
    }
  }
}

}

#endif
