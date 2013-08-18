@implementation CocoaRadioButton : NSButton

-(id) initWith:(phoenix::RadioButton&)radioButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    radioButton = &radioButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setButtonType:NSRadioButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  radioButton->setChecked();
  if(radioButton->onActivate) radioButton->onActivate();
}

@end

namespace phoenix {

bool pRadioButton::checked() {
  @autoreleasepool {
    return [cocoaView state] != NSOffState;
  }
}

Size pRadioButton::minimumSize() {
  Size size = Font::size(radioButton.font(), radioButton.state.text);
  return {size.width + 22, size.height};
}

void pRadioButton::setChecked() {
  @autoreleasepool {
    for(auto& button : radioButton.state.group) {
      auto state = (&button == &radioButton) ? NSOnState : NSOffState;
      [button.p.cocoaView setState:state];
    }
  }
}

void pRadioButton::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 1, geometry.y,
    geometry.width + 2, geometry.height
  });
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
}

void pRadioButton::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pRadioButton::constructor() {
  @autoreleasepool {
    cocoaView = cocoaRadioButton = [[CocoaRadioButton alloc] initWith:radioButton];
  }
}

void pRadioButton::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
