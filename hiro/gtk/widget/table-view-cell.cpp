#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewCell::construct() -> void {
  _setState();
}

auto pTableViewCell::destruct() -> void {
}

auto pTableViewCell::setAlignment(Alignment alignment) -> void {
}

auto pTableViewCell::setBackgroundColor(Color color) -> void {
}

auto pTableViewCell::setCheckable(bool checkable) -> void {
}

auto pTableViewCell::setChecked(bool checked) -> void {
  _setState();
}

auto pTableViewCell::setForegroundColor(Color color) -> void {
}

auto pTableViewCell::setIcon(const image& icon) -> void {
  _setState();
}

auto pTableViewCell::setText(const string& text) -> void {
  _setState();
}

auto pTableViewCell::_grandparent() -> maybe<pTableView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pTableViewCell::_parent() -> maybe<pTableViewItem&> {
  if(auto parent = self().parentTableViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewCell::_setState() -> void {
  if(auto parent = _parent()) {
    if(auto grandparent = _grandparent()) {
      if(self().offset() < grandparent->self().columnCount()) {
        auto lock = grandparent->acquire();
        gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 0, state().checked, -1);
        gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 1, CreatePixbuf(state().icon), -1);
        gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 2, state().text.data(), -1);
      }
    }
  }
}

}

#endif
