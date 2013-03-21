@implementation CocoaMenu : NSMenuItem

-(id) initWith:(phoenix::Menu&)menuReference {
  if(self = [super initWithTitle:@"" action:nil keyEquivalent:@""]) {
    menu = &menuReference;

    cocoaMenu = [[NSMenu alloc] initWithTitle:@""];
    [self setSubmenu:cocoaMenu];
  }
  return self;
}

-(NSMenu*) cocoaMenu {
  return cocoaMenu;
}

@end

namespace phoenix {

void pMenu::append(Action &action) {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] addItem:action.p.cocoaAction];
  }
}

void pMenu::remove(Action &action) {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] removeItem:action.p.cocoaAction];
  }
}

void pMenu::setImage(const image &image) {
  @autoreleasepool {
    unsigned size = 15;  //there is no API to retrieve the optimal size
    [cocoaAction setImage:NSMakeImage(image, size, size)];
  }
}

void pMenu::setText(const string &text) {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] setTitle:[NSString stringWithUTF8String:text]];
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pMenu::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaMenu = [[CocoaMenu alloc] initWith:menu];
    setText(menu.state.text);
  }
}

void pMenu::destructor() {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] release];
    [cocoaAction release];
  }
}

}
