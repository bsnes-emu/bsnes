#if defined(Hiro_MenuItem)

@implementation CocoaMenuItem : NSMenuItem

-(id) initWith:(hiro::mMenuItem&)menuItemReference {
  if(self = [super initWithTitle:@"" action:@selector(activate) keyEquivalent:@""]) {
    menuItem = &menuItemReference;

    [self setTarget:self];
  }
  return self;
}

-(void) activate {
  menuItem->doActivate();
}

@end

namespace hiro {

auto pMenuItem::construct() -> void {
  @autoreleasepool {
    cocoaAction = cocoaMenuItem = [[CocoaMenuItem alloc] initWith:self()];
    pAction::construct();

    setImage(state().image);
    setText(state().text);
  }
}

auto pMenuItem::destruct() -> void {
  @autoreleasepool {
    [cocoaAction release];
  }
}

auto pMenuItem::setImage(const Image& image) -> void {
  @autoreleasepool {
    uint size = 15;  //there is no API to retrieve the optimal size
    [cocoaAction setImage:NSMakeImage(image, size, size)];
  }
}

auto pMenuItem::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
