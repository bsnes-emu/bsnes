@implementation CocoaSeparator : NSMenuItem

-(id) initWith :(phoenix::Separator&)separatorReference {
  if(self = [super separatorItem]) {
    separator = &separatorReference;
  }
  return self;
}

@end

namespace phoenix {

void pSeparator::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaSeparator = [[CocoaSeparator alloc] initWith:separator];
  }
}

void pSeparator::destructor() {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}
