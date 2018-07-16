#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  @autoreleasepool {
    return [[NSScreen screens] count];
  }
}

auto pMonitor::dpi(uint monitor) -> Position {
  @autoreleasepool {
    NSScreen* screen = [[NSScreen screens] objectAtIndex:monitor];
    NSDictionary* dictionary = [screen deviceDescription];
    NSSize dpi = [[dictionary objectForKey:NSDeviceSize] sizeValue];
    return {dpi.width, dpi.height};
  }
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  @autoreleasepool {
    NSRect rectangle = [[[NSScreen screens] objectAtIndex:monitor] frame];
    return {
      (int)rectangle.origin.x,
      (int)rectangle.origin.y,
      (int)rectangle.size.width,
      (int)rectangle.size.height
    };
  }
}

auto pMonitor::primary() -> uint {
  //on macOS, the primary monitor is always the first monitor
  return 0;
}

auto pMonitor::workspace(uint monitor) -> Geometry {
  @autoreleasepool {
    NSRect size = [[[NSScreen screens] objectAtIndex:monitor] frame];
    NSRect area = [[[NSScreen screens] objectAtIndex:monitor] visibleFrame];
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
