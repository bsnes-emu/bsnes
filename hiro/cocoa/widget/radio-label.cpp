@implementation CocoaRadioLabel : NSButton

-(id) initWith:(phoenix::RadioLabel&)radioLabelReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    radioLabel = &radioLabelReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setButtonType:NSRadioButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  radioLabel->setChecked();
  if(radioLabel->onActivate) radioLabel->onActivate();
}

@end

namespace phoenix {

Size pRadioLabel::minimumSize() {
  Size size = Font::size(radioLabel.font(), radioLabel.state.text);
  return {size.width + 22, size.height};
}

void pRadioLabel::setChecked() {
  @autoreleasepool {
    for(auto& item : radioLabel.state.group) {
      auto state = (&item == &radioLabel) ? NSOnState : NSOffState;
      [item.p.cocoaView setState:state];
    }
  }
}

void pRadioLabel::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 1, geometry.y,
    geometry.width + 2, geometry.height
  });
}

void pRadioLabel::setGroup(const group<RadioLabel>& group) {
}

void pRadioLabel::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pRadioLabel::constructor() {
  @autoreleasepool {
    cocoaView = cocoaRadioLabel = [[CocoaRadioLabel alloc] initWith:radioLabel];
  }
}

void pRadioLabel::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
