#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewColumn::construct() -> void {
  @autoreleasepool {
    if(auto tableView = _grandparent()) {
      [tableView->cocoaView reloadColumns];
    }
  }
}

auto pTableViewColumn::destruct() -> void {
  @autoreleasepool {
    if(auto tableView = _grandparent()) {
      [tableView->cocoaView reloadColumns];
    }
  }
}

auto pTableViewColumn::setActive() -> void {
}

auto pTableViewColumn::setAlignment(Alignment alignment) -> void {
}

auto pTableViewColumn::setBackgroundColor(Color color) -> void {
}

auto pTableViewColumn::setEditable(bool editable) -> void {
}

auto pTableViewColumn::setExpandable(bool expandable) -> void {
}

auto pTableViewColumn::setFont(const Font& font) -> void {
}

auto pTableViewColumn::setForegroundColor(Color color) -> void {
}

auto pTableViewColumn::setHorizontalAlignment(double alignment) -> void {
}

auto pTableViewColumn::setIcon(const image& icon) -> void {
}

auto pTableViewColumn::setResizable(bool resizable) -> void {
}

auto pTableViewColumn::setSortable(bool sortable) -> void {
}

auto pTableViewColumn::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto pTableView = _grandparent()) {
      NSTableColumn* tableColumn = [[pTableView->cocoaView content] tableColumnWithIdentifier:[[NSNumber numberWithInteger:self().offset()] stringValue]];
      [[tableColumn headerCell] setStringValue:[NSString stringWithUTF8STring:text]];
      [[pTableView->cocoaView headerView] setNeedsDisplay:YES];
    }
  }
}

auto pTableViewColumn::setVerticalAlignment(double alignment) -> void {
}

auto pTableViewColumn::setVisible(bool visible) -> void {
}

auto pTableViewColumn::setWidth(signed width) -> void {
}

auto pTableViewColumn::_grandparent() -> maybe<pTableView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pTableViewColumn::_parent() -> maybe<pTableViewHeader&> {
  if(auto parent = self().parentTableViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
