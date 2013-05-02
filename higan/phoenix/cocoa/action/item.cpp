@implementation CocoaItem : NSMenuItem

-(id) initWith:(phoenix::Item&)itemReference {
  if(self = [super initWithTitle:@"" action:@selector(activate) keyEquivalent:@""]) {
    item = &itemReference;

    [self setTarget:self];
  }
  return self;
}

-(void) activate {
  if(item->onActivate) item->onActivate();
}

@end

namespace phoenix {

void pItem::setImage(const image& image) {
  @autoreleasepool {
    unsigned size = 15;  //there is no API to retrieve the optimal size
    [cocoaAction setImage:NSMakeImage(image, size, size)];
  }
}

void pItem::setText(const string& text) {
  @autoreleasepool {
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pItem::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaItem = [[CocoaItem alloc] initWith:item];
  }
}

void pItem::destructor() {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}
