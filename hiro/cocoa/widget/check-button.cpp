@implementation CocoaCheckButton : NSButton

-(id) initWith:(phoenix::CheckButton&)checkButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    checkButton = &checkButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setBezelStyle:NSRegularSquareBezelStyle];
    [self setButtonType:NSOnOffButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  checkButton->state.checked = [self state] != NSOffState;
  if(checkButton->onToggle) checkButton->onToggle();
}

@end

namespace phoenix {

Size pCheckButton::minimumSize() {
  Size size = Font::size(checkButton.font(), checkButton.state.text);

  if(checkButton.state.orientation == Orientation::Horizontal) {
    size.width += checkButton.state.image.width;
    size.height = max(checkButton.state.image.height, size.height);
  }

  if(checkButton.state.orientation == Orientation::Vertical) {
    size.width = max(checkButton.state.image.width, size.width);
    size.height += checkButton.state.image.height;
  }

  return {size.width + 20, size.height + 4};
}

void pCheckButton::setChecked(bool checked) {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

void pCheckButton::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y - 2,
    geometry.width + 4, geometry.height + 4
  });
}

void pCheckButton::setImage(const image& image, Orientation orientation) {
  @autoreleasepool {
    if(image.empty()) {
      [cocoaView setImage:nil];
      return;
    }

    [cocoaView setImage:NSMakeImage(image)];

    if(orientation == Orientation::Horizontal) [cocoaView setImagePosition:NSImageLeft];
    if(orientation == Orientation::Vertical  ) [cocoaView setImagePosition:NSImageAbove];
  }
}

void pCheckButton::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pCheckButton::constructor() {
  @autoreleasepool {
    cocoaView = cocoaCheckButton = [[CocoaCheckButton alloc] initWith:checkButton];
  }
}

void pCheckButton::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
