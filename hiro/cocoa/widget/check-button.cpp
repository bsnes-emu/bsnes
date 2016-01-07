#if defined(Hiro_CheckButton)

@implementation CocoaCheckButton : NSButton

-(id) initWith:(hiro::mCheckButton&)checkButtonReference {
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
  checkButton->doToggle();
}

@end

namespace hiro {

auto pCheckButton::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaCheckButton = [[CocoaCheckButton alloc] initWith:self()];
    pWidget::construct();

    setBordered(state().bordered);
    setChecked(state().checked);
    setIcon(state().icon);
    setOrientation(state().orientation);
    setText(state().text);
  }
}

auto pCheckButton::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pCheckButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width());
    size.setHeight(max(size.height(), state().icon.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width()));
    size.setHeight(size.height() + state().icon.height());
  }

  return {size.width() + (state().text ? 20 : 8), size.height() + 8};
}

auto pCheckButton::setBordered(bool bordered) -> void {
}

auto pCheckButton::setChecked(bool checked) -> void {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

auto pCheckButton::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y() - 2,
    geometry.width() + 4, geometry.height() + 4
  });
}

auto pCheckButton::setIcon(const image& icon) -> void {
  @autoreleasepool {
    [cocoaView setImage:NSMakeImage(icon)];
  }
}

auto pCheckButton::setOrientation(Orientation orientation) -> void {
  @autoreleasepool {
    if(orientation == Orientation::Horizontal) [cocoaView setImagePosition:NSImageLeft];
    if(orientation == Orientation::Vertical  ) [cocoaView setImagePosition:NSImageAbove];
  }
}

auto pCheckButton::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
