@implementation CocoaVerticalScroller : NSScroller

-(id) initWith :(phoenix::VerticalScroller&)verticalScrollerReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 1)]) {
    verticalScroller = &verticalScrollerReference;

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
  double d = 1.0 / verticalScroller->state.length;
  double f = d * verticalScroller->state.position;

  [self setDoubleValue:f];
  [self setKnobProportion:d];
}

-(IBAction) scroll :(id)sender {
  auto &state = verticalScroller->state;

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

  if(verticalScroller->onChange) verticalScroller->onChange();
}

@end

namespace phoenix {

Size pVerticalScroller::minimumSize() {
  @autoreleasepool {
    return {[NSScroller scrollerWidthForControlSize:NSRegularControlSize scrollerStyle:NSScrollerStyleLegacy], 32};
  }
}

unsigned pVerticalScroller::position() {
  @autoreleasepool {
    return [cocoaView doubleValue] * verticalScroller.state.length;
  }
}

void pVerticalScroller::setLength(unsigned length) {
  @autoreleasepool {
    [cocoaView update];
  }
}

void pVerticalScroller::setPosition(unsigned position) {
  @autoreleasepool {
    [cocoaView update];
  }
}

void pVerticalScroller::constructor() {
  @autoreleasepool {
    cocoaView = cocoaVerticalScroller = [[CocoaVerticalScroller alloc] initWith:verticalScroller];
  }
}

void pVerticalScroller::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
