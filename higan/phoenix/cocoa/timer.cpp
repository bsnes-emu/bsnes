@implementation CocoaTimer : NSObject

-(id) initWith :(phoenix::Timer&)timerReference {
  if(self = [super init]) {
    timer = &timerReference;
    instance = nil;
  }
  return self;
}

-(NSTimer*) instance {
  return instance;
}

-(void) update {
  if(instance) {
    [instance invalidate];
    instance = nil;
  }
  if(timer->state.enabled == false) return;
  instance = [NSTimer
    scheduledTimerWithTimeInterval:timer->state.milliseconds / 1000.0
    target:self selector:@selector(run:) userInfo:nil repeats:YES
  ];
}

-(void) run :(NSTimer*)instance {
  if(timer->onActivate) timer->onActivate();
}

@end

namespace phoenix {

void pTimer::setEnabled(bool enabled) {
  @autoreleasepool {
    [cocoaTimer update];
  }
}

void pTimer::setInterval(unsigned milliseconds) {
  @autoreleasepool {
    [cocoaTimer update];
  }
}

void pTimer::constructor() {
  @autoreleasepool {
    cocoaTimer = [[CocoaTimer alloc] initWith:timer];
  }
}

void pTimer::destructor() {
  @autoreleasepool {
    if([cocoaTimer instance]) [[cocoaTimer instance] invalidate];
    [cocoaTimer release];
  }
}

}
