#if defined(Hiro_Layout)

namespace hiro {

auto pLayout::construct() -> void {
  for(auto& sizable : state().sizables) sizable->construct();
}

auto pLayout::destruct() -> void {
  for(auto& sizable : state().sizables) sizable->destruct();
}

auto pLayout::setEnabled(bool enabled) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setEnabled(enabled && sizable->enabled(true));
  }
}

auto pLayout::setFont(const Font& font) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setFont(font ? font : sizable->font(true));
  }
}

auto pLayout::setVisible(bool visible) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setVisible(visible && sizable->visible(true));
  }
}

}

#endif
