#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
}

auto pListViewColumn::destruct() -> void {
}

auto pListViewColumn::setActive() -> void {
  //unsupported
}

auto pListViewColumn::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pListViewColumn::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pListViewColumn::setEditable(bool editable) -> void {
  //unsupported
}

auto pListViewColumn::setExpandable(bool expandable) -> void {
  _setState();
}

auto pListViewColumn::setFont(const Font& font) -> void {
  _setState();
}

auto pListViewColumn::setForegroundColor(Color color) -> void {
  _setState();
}

auto pListViewColumn::setHorizontalAlignment(double alignment) -> void {
  _setState();
}

auto pListViewColumn::setIcon(const image& icon) -> void {
  //unsupported
}

auto pListViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pListViewColumn::setSortable(bool sortable) -> void {
  _setState();
}

auto pListViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pListViewColumn::setVerticalAlignment(double alignment) -> void {
  _setState();
}

auto pListViewColumn::setVisible(bool visible) -> void {
  _setState();
}

auto pListViewColumn::setWidth(signed width) -> void {
  _setState();
}

auto pListViewColumn::_parent() -> maybe<pListViewHeader&> {
  if(auto parent = self().parentListViewHeader()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pListViewColumn::_setState() -> void {
  if(auto header = _parent()) {
    if(auto parent = header->_parent()) {
      parent->qtListView->header()->setResizeMode(self().offset(), state().resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
      bool clickable = false;
      for(auto& column : header->state().columns) clickable |= column->state.sortable;
      parent->qtListView->header()->setClickable(clickable);
      parent->qtListView->headerItem()->setText(self().offset(), QString::fromUtf8(state().text));
      parent->qtListView->setColumnHidden(self().offset(), !self().visible());

      for(auto& item : parent->state().items) {
        if(auto cell = item->cell(self().offset())) {
          if(auto self = cell->self()) self->_setState();
        }
      }
    }
  }
}

}

#endif
