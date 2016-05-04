#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewColumn::construct() -> void {
}

auto pTableViewColumn::destruct() -> void {
}

auto pTableViewColumn::setActive() -> void {
  //unsupported
}

auto pTableViewColumn::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pTableViewColumn::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pTableViewColumn::setEditable(bool editable) -> void {
  //unsupported
}

auto pTableViewColumn::setExpandable(bool expandable) -> void {
  _setState();
}

auto pTableViewColumn::setFont(const Font& font) -> void {
  _setState();
}

auto pTableViewColumn::setForegroundColor(Color color) -> void {
  _setState();
}

auto pTableViewColumn::setHorizontalAlignment(double alignment) -> void {
  _setState();
}

auto pTableViewColumn::setIcon(const image& icon) -> void {
  //unsupported
}

auto pTableViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pTableViewColumn::setSortable(bool sortable) -> void {
  _setState();
}

auto pTableViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pTableViewColumn::setVerticalAlignment(double alignment) -> void {
  _setState();
}

auto pTableViewColumn::setVisible(bool visible) -> void {
  _setState();
}

auto pTableViewColumn::setWidth(signed width) -> void {
  _setState();
}

auto pTableViewColumn::_parent() -> maybe<pTableViewHeader&> {
  if(auto parent = self().parentTableViewHeader()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pTableViewColumn::_setState() -> void {
  if(auto header = _parent()) {
    if(auto parent = header->_parent()) {
      parent->qtTableView->header()->setResizeMode(self().offset(), state().resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
      bool clickable = false;
      for(auto& column : header->state().columns) clickable |= column->state.sortable;
      parent->qtTableView->header()->setClickable(clickable);
      parent->qtTableView->headerItem()->setText(self().offset(), QString::fromUtf8(state().text));
      parent->qtTableView->setColumnHidden(self().offset(), !self().visible());

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
