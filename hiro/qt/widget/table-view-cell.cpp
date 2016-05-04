#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewCell::construct() -> void {
}

auto pTableViewCell::destruct() -> void {
}

auto pTableViewCell::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pTableViewCell::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pTableViewCell::setCheckable(bool checkable) -> void {
  _setState();
}

auto pTableViewCell::setChecked(bool checked) -> void {
  _setState();
}

auto pTableViewCell::setFont(const string& font) -> void {
  _setState();
}

auto pTableViewCell::setForegroundColor(Color color) -> void {
  _setState();
}

auto pTableViewCell::setIcon(const image& icon) -> void {
  _setState();
}

auto pTableViewCell::setText(const string& text) -> void {
  _setState();
}

auto pTableViewCell::_parent() -> maybe<pTableViewItem&> {
  if(auto parent = self().parentTableViewItem()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pTableViewCell::_setState() -> void {
  if(auto parent = _parent()) {
    if(auto grandparent = parent->_parent()) {
      grandparent->lock();
      parent->qtItem->setBackground(self().offset(), CreateBrush(self().backgroundColor(true)));
      if(state().checkable) {
        parent->qtItem->setCheckState(self().offset(), state().checked ? Qt::Checked : Qt::Unchecked);
      } else {
        //extremely unintuitive; but this is the only way to remove an existing checkbox from a cell
        parent->qtItem->setData(self().offset(), Qt::CheckStateRole, QVariant());
      }
      parent->qtItem->setFont(self().offset(), pFont::create(self().font(true)));
      parent->qtItem->setForeground(self().offset(), CreateBrush(self().foregroundColor(true)));
      parent->qtItem->setIcon(self().offset(), CreateIcon(state().icon));
      parent->qtItem->setText(self().offset(), QString::fromUtf8(state().text));
      parent->qtItem->setTextAlignment(self().offset(), CalculateAlignment(self().alignment(true)));
      grandparent->unlock();
    }
  }
}

}

#endif
