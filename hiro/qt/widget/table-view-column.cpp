#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewColumn::construct() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
  //parent->qtTableView->setAlternatingRowColors(parent->self().columnCount() >= 2);
    parent->qtTableView->setColumnCount(parent->self().columnCount());
    for(auto& column : parent->state().columns) {
      if(auto delegate = column->self()) {
        delegate->setIcon(delegate->state().icon);
        delegate->setText(delegate->state().text);  //also handles setSorting();
        delegate->_setState();
      }
    }
  }
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
  if(auto parent = _parent()) {
    parent->qtTableView->headerItem()->setIcon(self().offset(), CreateIcon(icon));
  }
}

auto pTableViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pTableViewColumn::setSorting(Sort) -> void {
  if(auto parent = _parent()) {
    string label = state().text;
    if(state().sorting == Sort::Ascending ) label.append(" \u25b4");
    if(state().sorting == Sort::Descending) label.append(" \u25be");
    parent->qtTableView->headerItem()->setText(self().offset(), QString::fromUtf8(label));
  }
}

auto pTableViewColumn::setText(const string&) -> void {
  if(auto parent = _parent()) {
    string label = state().text;
    if(state().sorting == Sort::Ascending ) label.append(" \u25b4");
    if(state().sorting == Sort::Descending) label.append(" \u25be");
    parent->qtTableView->headerItem()->setText(self().offset(), QString::fromUtf8(label));
  }
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

auto pTableViewColumn::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return {};
}

auto pTableViewColumn::_setState() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
    parent->qtTableView->header()->setSectionResizeMode(self().offset(), state().resizable ? QHeaderView::Interactive : QHeaderView::Fixed);
    parent->qtTableView->setColumnHidden(self().offset(), !self().visible());

    for(auto& item : parent->state().items) {
      if(auto cell = item->cell(self().offset())) {
        if(auto self = cell->self()) self->_setState();
      }
    }
  }
}

}

#endif
