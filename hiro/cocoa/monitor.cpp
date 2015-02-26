namespace phoenix {

unsigned pMonitor::count() {
  @autoreleasepool {
    return [[NSScreen screens] count];
  }
}

Geometry pMonitor::geometry(unsigned monitor) {
  @autoreleasepool {
    NSRect rectangle = [[[NSScreen screens] objectAtIndex:monitor] frame];
    return {rectangle.origin.x, rectangle.origin.y, rectangle.size.width, rectangle.size.height};
  }
}

unsigned pMonitor::primary() {
  //on OS X, the primary monitor is always the first monitor
  return 0;
}

}
