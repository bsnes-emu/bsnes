#if defined(Hiro_ListView)

namespace hiro {

auto pListViewHeader::construct() -> void {
}

auto pListViewHeader::destruct() -> void {
}

auto pListViewHeader::append(sListViewColumn column) -> void {
}

auto pListViewHeader::remove(sListViewColumn column) -> void {
}

auto pListViewHeader::setVisible(bool visible) -> void {
  @autoreleasepool {
    if(auto pListView = _parent()) {
      if(visible) {
        [[pListView->cocoaView content] setHeaderView:[[[NSTableHeaderView alloc] init] autorelease]];
      } else {
        [[pListView->cocoaView content] setHeaderView:nil];
      }
    }
  }
}

auto pListViewHeader::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
