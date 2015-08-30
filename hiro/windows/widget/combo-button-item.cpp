#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButtonItem::construct() -> void {
}

auto pComboButtonItem::destruct() -> void {
}

auto pComboButtonItem::setImage(const Image& image) -> void {
  //unsupported
}

auto pComboButtonItem::setSelected() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    SendMessage(parent->hwnd, CB_SETCURSEL, self().offset(), 0);
    parent->unlock();
  }
}

auto pComboButtonItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    parent->lock();
    SendMessage(parent->hwnd, CB_DELETESTRING, self().offset(), 0);
    SendMessage(parent->hwnd, CB_INSERTSTRING, self().offset(), (LPARAM)(wchar_t*)utf16_t(state().text));
    if(state().selected) setSelected();
    parent->unlock();
  }
}

auto pComboButtonItem::_parent() -> maybe<pComboButton&> {
  if(auto parent = self().parentComboButton()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
