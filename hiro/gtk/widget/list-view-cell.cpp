#if defined(Hiro_ListView)

namespace hiro {

auto pListViewCell::construct() -> void {
  _setState();
}

auto pListViewCell::destruct() -> void {
}

auto pListViewCell::setAlignment(Alignment alignment) -> void {
}

auto pListViewCell::setBackgroundColor(Color color) -> void {
}

auto pListViewCell::setCheckable(bool checkable) -> void {
}

auto pListViewCell::setChecked(bool checked) -> void {
  _setState();
}

auto pListViewCell::setForegroundColor(Color color) -> void {
}

auto pListViewCell::setIcon(const image& icon) -> void {
  _setState();
}

auto pListViewCell::setText(const string& text) -> void {
  _setState();
}

auto pListViewCell::_grandparent() -> maybe<pListView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pListViewCell::_parent() -> maybe<pListViewItem&> {
  if(auto parent = self().parentListViewItem()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewCell::_setState() -> void {
  if(auto parent = _parent()) {
    if(auto grandparent = _grandparent()) {
      grandparent->lock();
      gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 0, state().checked, -1);
      gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 1, CreatePixbuf(state().icon), -1);
      gtk_list_store_set(grandparent->gtkListStore, &parent->gtkIter, 3 * self().offset() + 2, state().text.data(), -1);
      grandparent->unlock();
    }
  }
}

}

#endif
