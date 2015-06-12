#if defined(Hiro_ListView)

namespace hiro {

auto pListViewCell::construct() -> void {
}

auto pListViewCell::destruct() -> void {
}

auto pListViewCell::setBackgroundColor(Color color) -> void {
}

auto pListViewCell::setForegroundColor(Color color) -> void {
}

auto pListViewCell::setIcon(const image& icon) -> void {
  if(auto item = _parent()) {
    if(auto view = item->_parent()) {
      gtk_list_store_set(view->gtkListStore, &item->gtkIter, 1 + self().offset() * 2, CreatePixbuf(icon), -1);
    }
  }
}

auto pListViewCell::setText(const string& text) -> void {
  if(auto item = _parent()) {
    if(auto view = item->_parent()) {
      gtk_list_store_set(view->gtkListStore, &item->gtkIter, 1 + self().offset() * 2 + 1, text.data(), -1);
    }
  }
}

auto pListViewCell::_parent() -> pListViewItem* {
  if(auto parent = self().parentListViewItem()) return parent->self();
  return nullptr;
}

}

#endif
