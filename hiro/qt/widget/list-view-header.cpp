#if defined(Hiro_ListView)

namespace hiro {

auto pListViewHeader::construct() -> void {
}

auto pListViewHeader::destruct() -> void {
}

auto pListViewHeader::append(sListViewColumn column) -> void {
  _setState();
}

auto pListViewHeader::remove(sListViewColumn column) -> void {
  _setState();
}

auto pListViewHeader::setVisible(bool visible) -> void {
  _setState();
}

auto pListViewHeader::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pListViewHeader::_setState() -> void {
  if(auto parent = _parent()) {
  //parent->qtListView->setAlternatingRowColors(self().columnCount() >= 2);
    parent->qtListView->setColumnCount(self().columnCount());
    parent->qtListView->setHeaderHidden(!self().visible());
    for(auto& column : state().columns) {
      if(auto self = column->self()) self->_setState();
    }
  }
}

}

#endif
