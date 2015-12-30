#if defined(Hiro_HorizontalScrollBar)

@implementation CocoaHorizontalScrollBar : NSScroller

-(id) initWith:(hiro::mHorizontalScrollBar&)horizontalScrollBarReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 1, 0)]) {
    horizontalScrollBar = &horizontalScrollBarReference;

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
  double d = 1.0 / horizontalScrollBar->state.length;
  double f = d * horizontalScrollBar->state.position;

  [self setDoubleValue:f];
  [self setKnobProportion:d];
}

-(IBAction) scroll:(id)sender {
  auto& state = horizontalScrollBar->state;

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

  horizontalScrollBar->doChange();
}

@end

namespace hiro {

auto pHorizontalScrollBar::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaHorizontalScrollBar = [[CocoaHorizontalScrollBar alloc] initWith:self()];
    pWidget::construct();

    setLength(state().length);
    setPosition(state().position);
  }
}

auto pHorizontalScrollBar::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pHorizontalScrollBar::minimumSize() const -> Size {
  @autoreleasepool {
    return {32, (int)[NSScroller scrollerWidthForControlSize:NSRegularControlSize scrollerStyle:NSScrollerStyleLegacy]};
  }
}

auto pHorizontalScrollBar::setLength(uint length) -> void {
  @autoreleasepool {
    [cocoaView update];
  }
}

auto pHorizontalScrollBar::setPosition(uint position) -> void {
  @autoreleasepool {
    [cocoaView update];
  }
}

}

#endif
