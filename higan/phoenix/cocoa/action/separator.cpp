namespace phoenix {

void pSeparator::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaSeparator = [[NSMenuItem separatorItem] retain];
  }
}

void pSeparator::destructor() {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}
