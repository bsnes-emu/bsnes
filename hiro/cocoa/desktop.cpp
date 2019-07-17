#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  @autoreleasepool {
    NSRect primary = [[[NSScreen screens] objectAtIndex:0] frame];
    return {
      (int)primary.size.width,
      (int)primary.size.height
    };
  }
}

auto pDesktop::workspace() -> Geometry {
  @autoreleasepool {
    auto screen = Desktop::size();
    NSRect area = [[[NSScreen screens] objectAtIndex:0] visibleFrame];
    return {
      (int)area.origin.x,
      (int)area.origin.y,
      (int)area.size.width,
      (int)area.size.height
    };
  }
}

}

#endif
