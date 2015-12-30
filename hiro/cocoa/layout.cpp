#if defined(Hiro_Layout)

namespace hiro {

auto pLayout::construct() -> void {
  for(auto& sizable : state().sizables) sizable->construct();
}

auto pLayout::destruct() -> void {
  for(auto& sizable : state().sizables) sizable->destruct();
}

}

#endif
