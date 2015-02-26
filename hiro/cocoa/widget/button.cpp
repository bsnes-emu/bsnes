@implementation CocoaButton : NSButton

-(id) initWith:(phoenix::Button&)buttonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    button = &buttonReference;
    [self setTarget:self];
    [self setAction:@selector(activate:)];
    //NSRoundedBezelStyle has a fixed height; which breaks both icons and larger/smaller text
    [self setBezelStyle:NSRegularSquareBezelStyle];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  if(button->onActivate) button->onActivate();
}

@end

namespace phoenix {

Size pButton::minimumSize() {
  Size size = Font::size(button.font(), button.state.text);

  if(button.state.orientation == Orientation::Horizontal) {
    size.width += button.state.image.width;
    size.height = max(button.state.image.height, size.height);
  }

  if(button.state.orientation == Orientation::Vertical) {
    size.width = max(button.state.image.width, size.width);
    size.height += button.state.image.height;
  }

  return {size.width + (button.state.text ? 20 : 4), size.height + 4};
}

void pButton::setBordered(bool bordered) {
}

void pButton::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y - 2,
    geometry.width + 4, geometry.height + 4
  });
}

void pButton::setImage(const image& image, Orientation orientation) {
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

void pButton::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pButton::constructor() {
  @autoreleasepool {
    cocoaView = cocoaButton = [[CocoaButton alloc] initWith:button];
  }
}

void pButton::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
