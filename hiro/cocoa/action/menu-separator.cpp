#if defined(Hiro_MenuSeparator)

namespace hiro {

auto pMenuSeparator::construct() -> void {
  @autoreleasepool {
    cocoaAction = cocoaSeparator = [[NSMenuItem separatorItem] retain];
    pAction::construct();
  }
}

auto pMenuSeparator::destruct() -> void {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}

#endif
