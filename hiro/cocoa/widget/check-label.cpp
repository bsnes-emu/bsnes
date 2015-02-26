@implementation CocoaCheckLabel : NSButton

-(id) initWith:(phoenix::CheckLabel&)checkLabelReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    checkLabel = &checkLabelReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setButtonType:NSSwitchButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  checkLabel->state.checked = [self state] != NSOffState;
  if(checkLabel->onToggle) checkLabel->onToggle();
}

@end

namespace phoenix {

Size pCheckLabel::minimumSize() {
  Size size = Font::size(checkLabel.font(), checkLabel.state.text);
  return {size.width + 20, size.height};
}

void pCheckLabel::setChecked(bool checked) {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

void pCheckLabel::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y,
    geometry.width + 4, geometry.height
  });
}

void pCheckLabel::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pCheckLabel::constructor() {
  @autoreleasepool {
    cocoaView = cocoaCheckLabel = [[CocoaCheckLabel alloc] initWith:checkLabel];
    setChecked(checkLabel.state.checked);
    setText(checkLabel.state.text);
  }
}

void pCheckLabel::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
