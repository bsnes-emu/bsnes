#if defined(Hiro_ListView)

namespace hiro {

auto pListViewHeader::construct() -> void {
  _setState();
}

auto pListViewHeader::destruct() -> void {
}

auto pListViewHeader::append(sListViewColumn column) -> void {
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
