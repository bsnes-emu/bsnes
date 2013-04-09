@implementation CocoaDelegate : NSObject

-(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication*)sender {
  using phoenix::Application;
  if(Application::Cocoa::onQuit) Application::Cocoa::onQuit();
  else Application::quit();
  return NSTerminateCancel;
}

-(BOOL) applicationShouldHandleReopen:(NSApplication*)application hasVisibleWindows:(BOOL)flag {
  using phoenix::Application;
  if(Application::Cocoa::onActivate) Application::Cocoa::onActivate();
  return NO;
}

-(void) run:(NSTimer*)timer {
  using phoenix::Application;
  if(Application::main) Application::main();
}

-(void) updateInDock:(NSTimer*)timer {
  NSArray *windows = [NSApp windows];
  for(unsigned n = 0; n < [windows count]; n++) {
    NSWindow *window = [windows objectAtIndex:n];
    if([window isMiniaturized]) {
      [window updateInDock];
    }
  }
}

@end

CocoaDelegate *cocoaDelegate = nullptr;

namespace phoenix {

void pApplication::run() {
//NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.1667 target:cocoaDelegate selector:@selector(updateInDock:) userInfo:nil repeats:YES];

  if(Application::main) {
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.0 target:cocoaDelegate selector:@selector(run:) userInfo:nil repeats:YES];

    //below line is needed to run application during window resize; however it has a large performance penalty on the resize smoothness
    //[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
  }

  @autoreleasepool {
    [NSApp run];
  }
}

bool pApplication::pendingEvents() {
  bool result = false;
  @autoreleasepool {
    NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:NO];
    if(event != nil) result = true;
  }
  return result;
}

void pApplication::processEvents() {
  @autoreleasepool {
    while(applicationState.quit == false) {
      NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
      if(event == nil) break;
      [event retain];
      [NSApp sendEvent:event];
      [event release];
    }
  }
}

void pApplication::quit() {
  @autoreleasepool {
    [NSApp stop:nil];
    NSEvent *event = [NSEvent otherEventWithType:NSApplicationDefined location:NSMakePoint(0, 0) modifierFlags:0 timestamp:0.0 windowNumber:0 context:nil subtype:0 data1:0 data2:0];
    [NSApp postEvent:event atStart:true];
  }
}

void pApplication::initialize() {
  @autoreleasepool {
    [NSApplication sharedApplication];
    cocoaDelegate = [[CocoaDelegate alloc] init];
    [NSApp setDelegate:cocoaDelegate];
    //every window has the default application menu; call this so it is displayed at startup
    [NSApp setMainMenu:[Window::none().p.cocoaWindow menu]];
  }
}

}
