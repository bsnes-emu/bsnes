#if defined(Hiro_ProgressBar)

@implementation CocoaProgressBar : NSProgressIndicator

-(id) initWith:(hiro::mProgressBar&)progressBarReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    progressBar = &progressBarReference;

    [self setIndeterminate:NO];
    [self setMinValue:0.0];
    [self setMaxValue:100.0];
  }
  return self;
}

@end

namespace hiro {

auto pProgressBar::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaProgressBar = [[CocoaProgressBar alloc] initWith:self()];
    pWidget::construct();

    setPosition(state().position);
  }
}

auto pProgressBar::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pProgressBar::minimumSize() const -> Size {
  return {0, 12};
}

auto pProgressBar::setPosition(uint position) -> void {
  @autoreleasepool {
    [cocoaView setDoubleValue:position];
  }
}

}

#endif
