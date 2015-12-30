#if defined(Hiro_VerticalScrollBar)

@implementation CocoaVerticalScrollBar : NSScroller

-(id) initWith:(hiro::mVerticalScrollBar&)verticalScrollBarReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 1)]) {
    verticalScrollBar = &verticalScrollBarReference;

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
  double d = 1.0 / verticalScrollBar->state.length;
  double f = d * verticalScrollBar->state.position;

  [self setDoubleValue:f];
  [self setKnobProportion:d];
}

-(IBAction) scroll:(id)sender {
  auto& state = verticalScrollBar->state;

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

  verticalScrollBar->doChange();
}

@end

namespace hiro {

auto pVerticalScrollBar::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaVerticalScrollBar = [[CocoaVerticalScrollBar alloc] initWith:self()];
    pWidget::construct();

    setLength(state().length);
    setPosition(state().position);
  }
}

auto pVerticalScrollBar::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pVerticalScrollBar::minimumSize() const -> Size {
  @autoreleasepool {
    return {(int)[NSScroller scrollerWidthForControlSize:NSRegularControlSize scrollerStyle:NSScrollerStyleLegacy], 32};
  }
}

auto pVerticalScrollBar::setLength(uint length) -> void {
  @autoreleasepool {
    [cocoaView update];
  }
}

auto pVerticalScrollBar::setPosition(uint position) -> void {
  @autoreleasepool {
    [cocoaView update];
  }
}

}

#endif
