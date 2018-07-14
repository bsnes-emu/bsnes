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

auto pTableViewColumn::setParent(mObject* parent, int offset) -> void {
  if(auto header = _parent()) header->_setState();
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
      auto lock = parent->acquire();
      #if HIRO_QT==4
      parent->qtTableView->header()->setResizeMode(self().offset(), state().resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
      #elif HIRO_QT==5
      parent->qtTableView->header()->setSectionResizeMode(self().offset(), state().resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
      #endif
      bool clickable = false;
      for(auto& column : header->state().columns) clickable |= column->state.sortable;
      #if HIRO_QT==4
      parent->qtTableView->header()->setClickable(clickable);
      #elif HIRO_QT==5
      parent->qtTableView->header()->setSectionsClickable(clickable);
      #endif
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
