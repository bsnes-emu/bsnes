#if defined(Hiro_Menu)

@implementation CocoaMenu : NSMenuItem

-(id) initWith:(hiro::mMenu&)menuReference {
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

namespace hiro {

auto pMenu::construct() -> void {
  @autoreleasepool {
    cocoaAction = cocoaMenu = [[CocoaMenu alloc] initWith:self()];
    pAction::construct();

    setImage(state().image);
    setText(state().text);
  }
}

auto pMenu::destruct() -> void {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] release];
    [cocoaAction release];
  }
}

auto pMenu::append(sAction action) -> void {
  @autoreleasepool {
    if(auto pAction = action->self()) {
      [[cocoaAction cocoaMenu] addItem:pAction->cocoaAction];
    }
  }
}

auto pMenu::remove(sAction action) -> void {
  @autoreleasepool {
    if(auto pAction = action->self()) {
      [[cocoaAction cocoaMenu] removeItem:pAction->cocoaAction];
    }
  }
}

auto pMenu::setImage(const Image& image) -> void {
  @autoreleasepool {
    uint size = 15;  //there is no API to retrieve the optimal size
    [cocoaAction setImage:NSMakeImage(image, size, size)];
  }
}

auto pMenu::setText(const string& text) -> void {
  @autoreleasepool {
    [[cocoaAction cocoaMenu] setTitle:[NSString stringWithUTF8String:text]];
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
