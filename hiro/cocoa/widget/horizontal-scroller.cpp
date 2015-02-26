@implementation CocoaHorizontalScroller : NSScroller

-(id) initWith:(phoenix::HorizontalScroller&)horizontalScrollerReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 1, 0)]) {
    horizontalScroller = &horizontalScrollerReference;

    [self setTarget:self];
    [self setAction:@selector(scroll:)];

    [self setControlSize:NSRegularControlSize];
    [self setScrollerStyle:NSScrollerStyleLegacy];
    [self setEnabled:YES];

    [self update];
  }
  return self;
}

-(void) update {
  double d = 1.0 / horizontalScroller->state.length;
  double f = d * horizontalScroller->state.position;

  [self setDoubleValue:f];
  [self setKnobProportion:d];
}

-(IBAction) scroll:(id)sender {
  auto& state = horizontalScroller->state;

  switch([self hitPart]) {
  case NSScrollerIncrementLine:
  case NSScrollerIncrementPage:
    if(state.position < state.length - 1) state.position++;
    [self update];
    break;

  case NSScrollerDecrementLine:
  case NSScrollerDecrementPage:
    if(state.position) state.position--;
    [self update];
    break;

  case NSScrollerKnob:
    state.position = [self doubleValue] * state.length;
    break;
  }

  if(horizontalScroller->onChange) horizontalScroller->onChange();
}

@end

namespace phoenix {

Size pHorizontalScroller::minimumSize() {
  @autoreleasepool {
    return {32, [NSScroller scrollerWidthForControlSize:NSRegularControlSize scrollerStyle:NSScrollerStyleLegacy]};
  }
}

void pHorizontalScroller::setLength(unsigned length) {
  @autoreleasepool {
    [cocoaView update];
  }
}

void pHorizontalScroller::setPosition(unsigned position) {
  @autoreleasepool {
    [cocoaView update];
  }
}

void pHorizontalScroller::constructor() {
  @autoreleasepool {
    cocoaView = cocoaHorizontalScroller = [[CocoaHorizontalScroller alloc] initWith:horizontalScroller];
  }
}

void pHorizontalScroller::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
