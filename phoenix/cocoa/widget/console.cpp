@implementation CocoaConsole : NSScrollView

-(id) initWith:(phoenix::Console&)consoleReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    console = &consoleReference;
  }
  return self;
}

@end

namespace phoenix {

void pConsole::print(string text) {
}

void pConsole::reset() {
}

void pConsole::setPrompt(string prompt) {
}

void pConsole::constructor() {
  @autoreleasepool {
    cocoaView = cocoaConsole = [[CocoaConsole alloc] initWith:console];
  }
}

void pConsole::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
