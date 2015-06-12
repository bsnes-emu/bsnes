#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
}

auto pListViewColumn::destruct() -> void {
}

auto pListViewColumn::setActive() -> void {
  //unsupported
}

auto pListViewColumn::setBackgroundColor(Color color) -> void {
}

auto pListViewColumn::setEditable(bool editable) -> void {
  //unsupported
}

auto pListViewColumn::setExpandable(bool expandable) -> void {
}

auto pListViewColumn::setForegroundColor(Color color) -> void {
}

auto pListViewColumn::setHorizontalAlignment(double alignment) -> void {
  _setState();
}

auto pListViewColumn::setIcon(const image& icon) -> void {
  _setState();
}

auto pListViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pListViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pListViewColumn::setVerticalAlignment(double alignment) -> void {
}

auto pListViewColumn::setWidth(signed width) -> void {
  _setState();
}

auto pListViewColumn::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewColumn::_setState() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    parent->_setIcons();
    utf16_t text(state().text);
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    lvColumn.fmt = LVCFMT_CENTER;
    lvColumn.iSubItem = self().offset();
    lvColumn.iImage = self().offset();
    lvColumn.pszText = text;
    lvColumn.cx = _width;
    if(state().horizontalAlignment < 0.333) lvColumn.fmt = LVCFMT_LEFT;
    if(state().horizontalAlignment > 0.666) lvColumn.fmt = LVCFMT_RIGHT;
    if(state().icon) lvColumn.mask |= LVCF_IMAGE;
    if(!state().resizable) lvColumn.fmt |= LVCFMT_FIXED_WIDTH;
    ListView_SetColumn(parent->hwnd, self().offset(), &lvColumn);
    parent->unlock();
  }
}

}

#endif
