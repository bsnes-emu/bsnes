#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
  @autoreleasepool {
    if(auto listView = _grandparent()) {
      [listView->cocoaView reloadColumns];
    }
  }
}

auto pListViewColumn::destruct() -> void {
  @autoreleasepool {
    if(auto listView = _grandparent()) {
      [listView->cocoaView reloadColumns];
    }
  }
}

auto pListViewColumn::setActive() -> void {
}

auto pListViewColumn::setAlignment(Alignment alignment) -> void {
}

auto pListViewColumn::setBackgroundColor(Color color) -> void {
}

auto pListViewColumn::setEditable(bool editable) -> void {
}

auto pListViewColumn::setExpandable(bool expandable) -> void {
}

auto pListViewColumn::setFont(const Font& font) -> void {
}

auto pListViewColumn::setForegroundColor(Color color) -> void {
}

auto pListViewColumn::setHorizontalAlignment(double alignment) -> void {
}

auto pListViewColumn::setImage(const Image& image) -> void {
}

auto pListViewColumn::setResizable(bool resizable) -> void {
}

auto pListViewColumn::setSortable(bool sortable) -> void {
}

auto pListViewColumn::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto pListView = _grandparent()) {
      NSTableColumn* tableColumn = [[pListView->cocoaView content] tableColumnWithIdentifier:[[NSNumber numberWithInteger:self().offset()] stringValue]];
      [[tableColumn headerCell] setStringValue:[NSString stringWithUTF8STring:text]];
      [[pListView->cocoaView headerView] setNeedsDisplay:YES];
    }
  }
}

auto pListViewColumn::setVerticalAlignment(double alignment) -> void {
}

auto pListViewColumn::setVisible(bool visible) -> void {
}

auto pListViewColumn::setWidth(signed width) -> void {
}

auto pListViewColumn::_grandparent() -> maybe<pListView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pListViewColumn::_parent() -> maybe<pListViewHeader&> {
  if(auto parent = self().parentListViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
