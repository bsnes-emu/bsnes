#if defined(Hiro_CheckLabel)

@implementation CocoaCheckLabel : NSButton

-(id) initWith:(hiro::mCheckLabel&)checkLabelReference {
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
  checkLabel->doToggle();
}

@end

namespace hiro {

auto pCheckLabel::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaCheckLabel = [[CocoaCheckLabel alloc] initWith:self()];
    pWidget::construct();

    setChecked(state().checked);
    setText(state().text);
  }
}

auto pCheckLabel::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pCheckLabel::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width() + 20, size.height()};
}

auto pCheckLabel::setChecked(bool checked) -> void {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

auto pCheckLabel::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y(),
    geometry.width() + 4, geometry.height()
  });
}

auto pCheckLabel::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
