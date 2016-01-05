#if defined(Hiro_MenuBar)

namespace hiro {

auto pMenuBar::construct() -> void {
}

auto pMenuBar::destruct() -> void {
}

auto pMenuBar::append(sMenu menu) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      if(auto pMenu = menu->self()) {
        [[parent->cocoaWindow menuBar] addItem:pMenu->cocoaAction];
      }
    }
  }
}

auto pMenuBar::remove(sMenu menu) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      if(auto pMenu = menu->self()) {
        [[parent->cocoaWindow menuBar] removeItem:pMenu->cocoaAction];
      }
    }
  }
}

auto pMenuBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
