#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewHeader::construct() -> void {
  _setState();
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
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewHeader::_setState() -> void {
  if(auto parent = _parent()) {
    gtk_tree_view_set_headers_visible(parent->gtkTreeView, self().visible());
    for(auto& column : state().columns) {
      if(auto self = column->self()) self->_setState();
    }
  }
}

}

#endif
