#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
}

auto pListViewItem::destruct() -> void {
}

auto pListViewItem::append(sListViewCell cell) -> void {
  @autoreleasepool {
    if(auto listView = _parent()) {
      [[listView->cocoaView content] reloadData];
    }
  }
}

auto pListViewItem::remove(sListViewCell cell) -> void {
  @autoreleasepool {
    if(auto listView = _parent()) {
      [[listView->cocoaView content] reloadData];
    }
  }
}

auto pListViewItem::setAlignment(Alignment alignment) -> void {
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
}

auto pListViewItem::setFocused() -> void {
}

auto pListViewItem::setForegroundColor(Color color) -> void {
}

auto pListViewItem::setSelected(bool selected) -> void {
}

auto pListViewItem::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
