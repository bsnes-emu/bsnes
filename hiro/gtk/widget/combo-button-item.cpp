#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButtonItem::construct() -> void {
  if(auto parent = _parent()) parent->_append(self());
}

auto pComboButtonItem::destruct() -> void {
}

auto pComboButtonItem::setIcon(const image& icon) -> void {
  if(auto parent = _parent()) {
    auto pixbuf = CreatePixbuf(icon, true);
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 0, pixbuf, -1);
  }
}

auto pComboButtonItem::setSelected() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    gtk_combo_box_set_active(parent->gtkComboBox, self().offset());
    parent->unlock();
  }
}

auto pComboButtonItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 1, text.data(), -1);
  }
}

auto pComboButtonItem::_parent() -> maybe<pComboButton&> {
  if(auto parent = self().parentComboButton()) {
    if(auto self = parent->self()) return *self;
  }
  return {};
}

}

#endif
