#if defined(Hiro_MenuCheckItem)

namespace hiro {

auto pMenuCheckItem::construct() -> void {
}

auto pMenuCheckItem::destruct() -> void {
}

auto pMenuCheckItem::setChecked(bool checked) -> void {
  if(auto menu = _parentMenu()) {
    CheckMenuItem(menu->hmenu, position, MF_BYPOSITION | (checked ? MF_CHECKED : MF_UNCHECKED));
  }
}

auto pMenuCheckItem::setText(const string& text) -> void {
  _synchronize();
}

auto pMenuCheckItem::onToggle() -> void {
  state().checked = !state().checked;
  setChecked(state().checked);
  self().doToggle();
}

}

#endif
