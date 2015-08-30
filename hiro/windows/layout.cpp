#if defined(Hiro_Layout)

namespace hiro {

auto pLayout::construct() -> void {
}

auto pLayout::destruct() -> void {
}

auto pLayout::setEnabled(bool enabled) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setEnabled(sizable->enabled(true));
  }
}

auto pLayout::setFont(const Font& font) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setFont(sizable->font(true));
  }
}

auto pLayout::setVisible(bool visible) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setVisible(sizable->visible(true));
  }
}

}

#endif
