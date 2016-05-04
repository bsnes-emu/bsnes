#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewHeader::construct() -> void {
  _setState();
}

auto pTableViewHeader::destruct() -> void {
}

auto pTableViewHeader::append(sTableViewColumn column) -> void {
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
    auto style = GetWindowLong(parent->hwnd, GWL_STYLE);
    self().visible() ? style &=~ LVS_NOCOLUMNHEADER : style |= LVS_NOCOLUMNHEADER;
    SetWindowLong(parent->hwnd, GWL_STYLE, style);
    for(auto& column : state().columns) {
      if(auto self = column->self()) self->_setState();
    }
  }
}

}

#endif
