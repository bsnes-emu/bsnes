#if defined(Hiro_Layout)

namespace hiro {

auto pLayout::construct() -> void {
  for(auto& sizable : state().sizables) sizable->construct();
}

auto pLayout::destruct() -> void {
  for(auto& sizable : state().sizables) sizable->destruct();
}

auto pLayout::setEnabled(bool enabled) -> void {
}

auto pLayout::setFont(const string& font) -> void {
}

auto pLayout::setVisible(bool visible) -> void {
}

}

#endif
