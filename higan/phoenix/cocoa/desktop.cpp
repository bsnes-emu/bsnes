namespace phoenix {

Size pDesktop::size() {
  @autoreleasepool {
    NSRect primary = [[[NSScreen screens] objectAtIndex:0] frame];
    return {primary.size.width, primary.size.height};
  }
}

Geometry pDesktop::workspace() {
  @autoreleasepool {
    auto screen = Desktop::size();
    NSRect area = [[[NSScreen screens] objectAtIndex:0] visibleFrame];
    return {area.origin.x, screen.height - area.size.height - area.origin.y, area.size.width, area.size.height};
  }
}

}
