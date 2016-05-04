#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewHeader::construct() -> void {
}

auto pTableViewHeader::destruct() -> void {
}

auto pTableViewHeader::append(sTableViewColumn column) -> void {
}

auto pTableViewHeader::remove(sTableViewColumn column) -> void {
}

auto pTableViewHeader::setVisible(bool visible) -> void {
  @autoreleasepool {
    if(auto pTableView = _parent()) {
      if(visible) {
        [[pTableView->cocoaView content] setHeaderView:[[[NSTableHeaderView alloc] init] autorelease]];
      } else {
        [[pTableView->cocoaView content] setHeaderView:nil];
      }
    }
  }
}

auto pTableViewHeader::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
