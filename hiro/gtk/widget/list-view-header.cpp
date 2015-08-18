#if defined(Hiro_ListView)

namespace hiro {

auto pListViewHeader::construct() -> void {
  _setState();
}

auto pListViewHeader::destruct() -> void {
}

auto pListViewHeader::append(sListViewColumn column) -> void {
  _setState();
}

auto pListViewHeader::remove(sListViewColumn column) -> void {
}

auto pListViewHeader::setVisible(bool visible) -> void {
  _setState();
}

auto pListViewHeader::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewHeader::_setState() -> void {
  if(auto parent = _parent()) {
    gtk_tree_view_set_headers_visible(parent->gtkTreeView, self().visible());
    for(auto& column : state().columns) {
      if(auto self = column->self()) self->_setState();
    }
  }
}

}

#endif
