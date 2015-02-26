namespace hiro {

auto pLayout::construct() -> void {
  for(auto& sizable : state().sizables) sizable->construct();
}

auto pLayout::destruct() -> void {
  for(auto& sizable : state().sizables) sizable->destruct();
}

auto pLayout::setEnabled(bool enabled) -> void {
  for(auto& sizable : state().sizables) {
    if(sizable->self()) sizable->self()->setEnabled(sizable->enabled(true));
  }
}

auto pLayout::setFont(const string& font) -> void {
  for(auto& sizable : state().sizables) {
    if(sizable->self()) sizable->self()->setFont(sizable->font(true));
  }
}

auto pLayout::setVisible(bool visible) -> void {
  for(auto& sizable : state().sizables) {
    if(sizable->self()) sizable->self()->setVisible(sizable->visible(true));
  }
}

}
