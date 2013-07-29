@implementation CocoaCheckItem : NSMenuItem

-(id) initWith:(phoenix::CheckItem&)checkItemReference {
  if(self = [super initWithTitle:@"" action:@selector(activate) keyEquivalent:@""]) {
    checkItem = &checkItemReference;

    [self setTarget:self];
  }
  return self;
}

-(void) activate {
  checkItem->state.checked = !checkItem->state.checked;
  auto state = checkItem->state.checked ? NSOnState : NSOffState;
  [self setState:state];
  if(checkItem->onToggle) checkItem->onToggle();
}

@end

namespace phoenix {

bool pCheckItem::checked() {
  @autoreleasepool {
    return [cocoaAction state] != NSOffState;
  }
}

void pCheckItem::setChecked(bool checked) {
  @autoreleasepool {
    auto state = checked ? NSOnState : NSOffState;
    [cocoaAction setState:state];
  }
}

void pCheckItem::setText(string text) {
  @autoreleasepool {
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pCheckItem::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaCheckItem = [[CocoaCheckItem alloc] initWith:checkItem];
    setText(checkItem.state.text);
  }
}

void pCheckItem::destructor() {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}
