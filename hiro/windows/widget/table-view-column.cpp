#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewColumn::construct() -> void {
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

auto pTableViewColumn::destruct() -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->lock();
    ListView_DeleteColumn(grandparent->hwnd, self().offset());
    grandparent->unlock();
  }
}

auto pTableViewColumn::setActive() -> void {
  //unsupported
}

auto pTableViewColumn::setAlignment(Alignment alignment) -> void {
}

auto pTableViewColumn::setBackgroundColor(Color color) -> void {
}

auto pTableViewColumn::setEditable(bool editable) -> void {
  //unsupported
}

auto pTableViewColumn::setExpandable(bool expandable) -> void {
}

auto pTableViewColumn::setForegroundColor(Color color) -> void {
}

auto pTableViewColumn::setHorizontalAlignment(double alignment) -> void {
  _setState();
}

auto pTableViewColumn::setIcon(const image& icon) -> void {
  _setState();
}

auto pTableViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pTableViewColumn::setSortable(bool sortable) -> void {
}

auto pTableViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pTableViewColumn::setVerticalAlignment(double alignment) -> void {
}

auto pTableViewColumn::setWidth(signed width) -> void {
  _setState();
}

auto pTableViewColumn::_grandparent() -> maybe<pTableView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pTableViewColumn::_parent() -> maybe<pTableViewHeader&> {
  if(auto parent = self().parentTableViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewColumn::_setState() -> void {
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
    if(state().icon) lvColumn.mask |= LVCF_IMAGE;
    if(!state().resizable) lvColumn.fmt |= LVCFMT_FIXED_WIDTH;
    ListView_SetColumn(grandparent->hwnd, self().offset(), &lvColumn);
    grandparent->unlock();
  }
}

}

#endif
