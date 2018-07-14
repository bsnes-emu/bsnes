#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewHeader::construct() -> void {
}

auto pTableViewHeader::destruct() -> void {
}

auto pTableViewHeader::append(sTableViewColumn column) -> void {
  _setState();
}

auto pTableViewHeader::remove(sTableViewColumn column) -> void {
}

auto pTableViewHeader::setVisible(bool visible) -> void {
  _setState();
}

auto pTableViewHeader::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pTableViewHeader::_setState() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
  //parent->qtTableView->setAlternatingRowColors(self().columnCount() >= 2);
    parent->qtTableView->setColumnCount(self().columnCount());
    parent->qtTableView->setHeaderHidden(!self().visible());
    for(auto& column : state().columns) {
      if(auto self = column->self()) self->_setState();
    }
  }
}

}

#endif
