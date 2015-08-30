#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->lock();
    wchar_t text[] = L"";
    LVCOLUMN lvColumn{0};
    lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    lvColumn.iSubItem = self().offset();
    lvColumn.pszText = text;
    ListView_InsertColumn(grandparent->hwnd, self().offset(), &lvColumn);
    _setState();
    grandparent->unlock();
  }
}

auto pListViewColumn::destruct() -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->lock();
    ListView_DeleteColumn(grandparent->hwnd, self().offset());
    grandparent->unlock();
  }
}

auto pListViewColumn::setActive() -> void {
  //unsupported
}

auto pListViewColumn::setAlignment(Alignment alignment) -> void {
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

auto pListViewColumn::setImage(const Image& image) -> void {
  _setState();
}

auto pListViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pListViewColumn::setSortable(bool sortable) -> void {
}

auto pListViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pListViewColumn::setVerticalAlignment(double alignment) -> void {
}

auto pListViewColumn::setWidth(signed width) -> void {
  _setState();
}

auto pListViewColumn::_grandparent() -> maybe<pListView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pListViewColumn::_parent() -> maybe<pListViewHeader&> {
  if(auto parent = self().parentListViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewColumn::_setState() -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->lock();
    grandparent->_setIcons();
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
    if(state().image) lvColumn.mask |= LVCF_IMAGE;
    if(!state().resizable) lvColumn.fmt |= LVCFMT_FIXED_WIDTH;
    ListView_SetColumn(grandparent->hwnd, self().offset(), &lvColumn);
    grandparent->unlock();
  }
}

}

#endif
