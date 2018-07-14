#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewItem::construct() -> void {
}

auto pTableViewItem::destruct() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
    if(qtItem) {
      delete qtItem;
      qtItem = nullptr;
    }
  }
}

auto pTableViewItem::append(sTableViewCell cell) -> void {
  _setState();
}

auto pTableViewItem::remove(sTableViewCell cell) -> void {
  _setState();
}

auto pTableViewItem::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pTableViewItem::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pTableViewItem::setFont(const Font& font) -> void {
  _setState();
}

auto pTableViewItem::setForegroundColor(Color color) -> void {
  _setState();
}

auto pTableViewItem::setSelected(bool selected) -> void {
  _setState();
}

auto pTableViewItem::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pTableViewItem::_setState() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    qtItem->setSelected(state().selected);
    if(state().selected) {
      parent->qtTableView->setCurrentItem(qtItem);
    }
    for(auto& cell : state().cells) {
      if(auto self = cell->self()) self->_setState();
    }
  }
}

}

#endif
