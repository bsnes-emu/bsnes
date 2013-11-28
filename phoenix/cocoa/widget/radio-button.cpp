@implementation CocoaRadioButton : NSButton

-(id) initWith:(phoenix::RadioButton&)radioButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    radioButton = &radioButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setBezelStyle:NSRegularSquareBezelStyle];
    [self setButtonType:NSOnOffButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  bool wasChecked = radioButton->state.checked;
  radioButton->setChecked();
  if(wasChecked == false) {
    if(radioButton->onActivate) radioButton->onActivate();
  }
}

@end

namespace phoenix {

Size pRadioButton::minimumSize() {
  Size size = Font::size(radioButton.font(), radioButton.state.text);

  if(radioButton.state.orientation == Orientation::Horizontal) {
    size.width += radioButton.state.image.width;
    size.height = max(radioButton.state.image.height, size.height);
  }

  if(radioButton.state.orientation == Orientation::Vertical) {
    size.width = max(radioButton.state.image.width, size.width);
    size.height += radioButton.state.image.height;
  }

  return {size.width + 20, size.height + 4};
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
    geometry.x - 2, geometry.y - 2,
    geometry.width + 4, geometry.height + 4
  });
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
}

void pRadioButton::setImage(const image& image, Orientation orientation) {
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
