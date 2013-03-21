@implementation CocoaProgressBar : NSProgressIndicator

-(id) initWith:(phoenix::ProgressBar&)progressBarReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    progressBar = &progressBarReference;

    [self setIndeterminate:NO];
    [self setMinValue:0.0];
    [self setMaxValue:100.0];
  }
  return self;
}

@end

namespace phoenix {

void pProgressBar::setPosition(unsigned position) {
  @autoreleasepool {
    [cocoaView setDoubleValue:position];
  }
}

void pProgressBar::constructor() {
  @autoreleasepool {
    cocoaView = cocoaProgressBar = [[CocoaProgressBar alloc] initWith:progressBar];
    setPosition(progressBar.state.position);
  }
}

void pProgressBar::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
