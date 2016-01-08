#if defined(Hiro_Application)

@implementation CocoaDelegate : NSObject

-(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*)sender {
  using hiro::Application;
  if(Application::state.cocoa.onQuit) Application::Cocoa::doQuit();
  else Application::quit();
  return NSTerminateCancel;
}

-(BOOL) applicationShouldHandleReopen:(NSApplication*)application hasVisibleWindows:(BOOL)flag {
  using hiro::Application;
  if(Application::state.cocoa.onActivate) Application::Cocoa::doActivate();
  return NO;
}

-(void) run:(NSTimer*)timer {
  using hiro::Application;
  if(Application::state.onMain) Application::doMain();
}

-(void) updateInDock:(NSTimer*)timer {
  NSArray* windows = [NSApp windows];
  for(uint n = 0; n < [windows count]; n++) {
    NSWindow* window = [windows objectAtIndex:n];
    if([window isMiniaturized]) {
      [window updateInDock];
    }
  }
}

@end

CocoaDelegate* cocoaDelegate = nullptr;

namespace hiro {

auto pApplication::run() -> void {
//NSTimer* timer = [NSTimer scheduledTimerWithTimeInterval:0.1667 target:cocoaDelegate selector:@selector(updateInDock:) userInfo:nil repeats:YES];

  if(Application::state.onMain) {
    NSTimer* timer = [NSTimer scheduledTimerWithTimeInterval:0.0 target:cocoaDelegate selector:@selector(run:) userInfo:nil repeats:YES];

    //below line is needed to run application during window resize; however it has a large performance penalty on the resize smoothness
    //[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
  }

  @autoreleasepool {
    [NSApp run];
  }
}

auto pApplication::pendingEvents() -> bool {
  bool result = false;
  @autoreleasepool {
    NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:NO];
    if(event != nil) result = true;
  }
  return result;
}

auto pApplication::processEvents() -> void {
  @autoreleasepool {
    while(!Application::state.quit) {
      NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
      if(event == nil) break;
      [event retain];
      [NSApp sendEvent:event];
      [event release];
    }
  }
}

auto pApplication::quit() -> void {
  @autoreleasepool {
    [NSApp stop:nil];
    NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined location:NSMakePoint(0, 0) modifierFlags:0 timestamp:0.0 windowNumber:0 context:nil subtype:0 data1:0 data2:0];
    [NSApp postEvent:event atStart:true];
  }
}

auto pApplication::initialize() -> void {
  @autoreleasepool {
    [NSApplication sharedApplication];
    cocoaDelegate = [[CocoaDelegate alloc] init];
    [NSApp setDelegate:cocoaDelegate];
  }
}

}

#endif
