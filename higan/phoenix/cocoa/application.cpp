@implementation CocoaDelegate : NSObject

-(NSApplicationTerminateReply) applicationShouldTerminate :(NSApplication*)sender {
  using phoenix::Application;
  if(Application::Cocoa::onQuit) Application::Cocoa::onQuit();
  else Application::quit();
  return NSTerminateCancel;
}

-(void) run :(NSTimer*)timer {
  using phoenix::Application;
  if(Application::main) Application::main();
}

@end

CocoaDelegate *cocoaDelegate = nullptr;

namespace phoenix {

void pApplication::run() {
  if(Application::main) {
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.0 target:cocoaDelegate selector:@selector(run:) userInfo:nil repeats:YES];
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
