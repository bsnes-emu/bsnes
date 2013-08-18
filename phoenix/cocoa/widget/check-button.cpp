@implementation CocoaCheckButton : NSButton

-(id) initWith:(phoenix::CheckButton&)checkButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    checkButton = &checkButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setButtonType:NSSwitchButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  checkButton->state.checked = [self state] != NSOffState;
  if(checkButton->onToggle) checkButton->onToggle();
}

@end

namespace phoenix {

bool pCheckButton::checked() {
  @autoreleasepool {
    return [cocoaView state] != NSOffState;
  }
}

Size pCheckButton::minimumSize() {
  Size size = Font::size(checkButton.font(), checkButton.state.text);
  return {size.width + 20, size.height};
}

void pCheckButton::setChecked(bool checked) {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

void pCheckButton::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y,
    geometry.width + 4, geometry.height
  });
}

void pCheckButton::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pCheckButton::constructor() {
  @autoreleasepool {
    cocoaView = cocoaCheckButton = [[CocoaCheckButton alloc] initWith:checkButton];
    setChecked(checkButton.state.checked);
    setText(checkButton.state.text);
  }
}

void pCheckButton::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
