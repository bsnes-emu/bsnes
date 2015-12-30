#if defined(Hiro_Button)

@implementation CocoaButton : NSButton

-(id) initWith:(hiro::mButton&)buttonReference {
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
  button->doActivate();
}

@end

namespace hiro {

auto pButton::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaButton = [[CocoaButton alloc] initWith:self()];
    pWidget::construct();

    setBordered(state().bordered);
    setImage(state().image);
    setOrientation(state().orientation);
    setText(state().text);
  }
}

auto pButton::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().image.width());
    size.setHeight(max(size.height(), state().image.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().image.width()));
    size.setHeight(size.height() + state().image.height());
  }

  return {size.width() + (state().text ? 20 : 4), size.height() + 4};
}

auto pButton::setBordered(bool bordered) -> void {
}

auto pButton::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y() - 2,
    geometry.width() + 4, geometry.height() + 4
  });
}

auto pButton::setImage(const Image& image) -> void {
  @autoreleasepool {
    if(!image) {
      [cocoaView setImage:nil];
      return;
    }

    [cocoaView setImage:NSMakeImage(image)];
  }
}

auto pButton::setOrientation(Orientation orientation) -> void {
  @autoreleasepool {
    if(orientation == Orientation::Horizontal) [cocoaView setImagePosition:NSImageLeft];
    if(orientation == Orientation::Vertical  ) [cocoaView setImagePosition:NSImageAbove];
  }
}

auto pButton::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
