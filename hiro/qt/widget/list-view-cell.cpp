#if defined(Hiro_ListView)

namespace hiro {

auto pListViewCell::construct() -> void {
}

auto pListViewCell::destruct() -> void {
}

auto pListViewCell::setAlignment(Alignment alignment) -> void {
  _setState();
}

auto pListViewCell::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pListViewCell::setCheckable(bool checkable) -> void {
  _setState();
}

auto pListViewCell::setChecked(bool checked) -> void {
  _setState();
}

auto pListViewCell::setFont(const string& font) -> void {
  _setState();
}

auto pListViewCell::setForegroundColor(Color color) -> void {
  _setState();
}

auto pListViewCell::setImage(const Image& image) -> void {
  _setState();
}

auto pListViewCell::setText(const string& text) -> void {
  _setState();
}

auto pListViewCell::_parent() -> maybe<pListViewItem&> {
  if(auto parent = self().parentListViewItem()) {
    if(auto delegate = parent->self()) return *delegate;
  }
  return nothing;
}

auto pListViewCell::_setState() -> void {
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
      parent->qtItem->setIcon(self().offset(), CreateImage(state().image));
      parent->qtItem->setText(self().offset(), QString::fromUtf8(state().text));
      parent->qtItem->setTextAlignment(self().offset(), CalculateAlignment(self().alignment(true)));
      grandparent->unlock();
    }
  }
}

}

#endif
