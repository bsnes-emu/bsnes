#if defined(Hiro_Timer)

@implementation CocoaTimer : NSObject

-(id) initWith:(hiro::mTimer&)timerReference {
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
  if(timer->enabled()) {
    instance = [NSTimer
      scheduledTimerWithTimeInterval:timer->state.interval / 1000.0
      target:self selector:@selector(run:) userInfo:nil repeats:YES
    ];
  }
}

-(void) run:(NSTimer*)instance {
  if(timer->enabled()) {
    timer->doActivate();
  }
}

@end

namespace hiro {

auto pTimer::construct() -> void {
  @autoreleasepool {
    cocoaTimer = [[CocoaTimer alloc] initWith:self()];
  }
}

auto pTimer::destruct() -> void {
  @autoreleasepool {
    if([cocoaTimer instance]) [[cocoaTimer instance] invalidate];
    [cocoaTimer release];
  }
}

auto pTimer::setEnabled(bool enabled) -> void {
  @autoreleasepool {
    [cocoaTimer update];
  }
}

auto pTimer::setInterval(uint interval) -> void {
  @autoreleasepool {
    [cocoaTimer update];
  }
}

}

#endif
