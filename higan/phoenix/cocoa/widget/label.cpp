@implementation CocoaLabel : NSTextField

-(id) initWith:(phoenix::Label&)labelReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    label = &labelReference;

    [self setAlignment:NSLeftTextAlignment];
    [self setBordered:NO];
    [self setDrawsBackground:NO];
    [self setEditable:NO];
  }
  return self;
}

@end

namespace phoenix {

Size pLabel::minimumSize() {
  Size size = Font::size(label.font(), label.state.text);
  return {size.width + 6, size.height};
}

void pLabel::setText(const string &text) {
  @autoreleasepool {
    [cocoaView setStringValue:[NSString stringWithUTF8String:text]];
  }
}

void pLabel::constructor() {
  @autoreleasepool {
    cocoaView = cocoaLabel = [[CocoaLabel alloc] initWith:label];
    setText(label.state.text);
  }
}

void pLabel::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
