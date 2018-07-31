#if defined(Hiro_Object)

namespace hiro {

auto pObject::construct() -> void {
}

auto pObject::destruct() -> void {
}

auto pObject::reconstruct() -> void {
}

auto pObject::focused() const -> bool {
  return false;
}

auto pObject::remove() -> void {
}

auto pObject::reset() -> void {
}

auto pObject::setEnabled(bool enabled) -> void {
}

auto pObject::setFocused() -> void {
}

auto pObject::setFont(const Font& font) -> void {
}

auto pObject::setGroup(sGroup group) -> void {
}

auto pObject::setVisible(bool visible) -> void {
}

}

#endif
