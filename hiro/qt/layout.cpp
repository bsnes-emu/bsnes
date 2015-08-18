#if defined(Hiro_Layout)

namespace hiro {

auto pLayout::construct() -> void {
}

auto pLayout::destruct() -> void {
}

auto pLayout::setVisible(bool visible) -> void {
  for(auto& sizable : state().sizables) {
    if(auto self = sizable->self()) self->setVisible(sizable->visible(true));
  }
}

}

#endif
