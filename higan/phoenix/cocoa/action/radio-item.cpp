@implementation CocoaRadioItem : NSMenuItem

-(id) initWith :(phoenix::RadioItem&)radioItemReference {
  if(self = [super initWithTitle:@"" action:@selector(activate) keyEquivalent:@""]) {
    radioItem = &radioItemReference;

    [self setTarget:self];
    [self setOnStateImage:[NSImage imageNamed:@"NSMenuRadio"]];
  }
  return self;
}

-(void) activate {
  radioItem->setChecked();
  if(radioItem->onActivate) radioItem->onActivate();
}

@end

namespace phoenix {

bool pRadioItem::checked() {
  @autoreleasepool {
    return [cocoaAction state] != NSOffState;
  }
}

void pRadioItem::setChecked() {
  @autoreleasepool {
    for(auto &item : radioItem.state.group) {
      auto state = (&item == &radioItem) ? NSOnState : NSOffState;
      [item.p.cocoaAction setState:state];
    }
  }
}

void pRadioItem::setGroup(const set<RadioItem&> &group) {
}

void pRadioItem::setText(const string &text) {
  @autoreleasepool {
    [cocoaAction setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pRadioItem::constructor() {
  @autoreleasepool {
    cocoaAction = cocoaRadioItem = [[CocoaRadioItem alloc] initWith:radioItem];
    setText(radioItem.state.text);
  }
}

void pRadioItem::destructor() {
  @autoreleasepool {
    [cocoaAction release];
  }
}

}
