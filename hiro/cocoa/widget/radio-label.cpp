#if defined(Hiro_RadioLabel)

@implementation CocoaRadioLabel : NSButton

-(id) initWith:(hiro::mRadioLabel&)radioLabelReference {
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
  radioLabel->doActivate();
}

@end

namespace hiro {

auto pRadioLabel::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaRadioLabel = [[CocoaRadioLabel alloc] initWith:self()];
    pWidget::construct();

    if(state().checked) setChecked();
    setText(state().text);
  }
}

auto pRadioLabel::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pRadioLabel::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width() + 22, size.height()};
}

auto pRadioLabel::setChecked() -> void {
  @autoreleasepool {
    if(auto group = state().group) {
      for(auto& weak : group->state.objects) {
        if(auto object = weak.acquire()) {
          if(auto self = object->self()) {
            if(auto p = dynamic_cast<pRadioLabel*>(self)) {
              auto state = this == p ? NSOnState : NSOffState;
              [p->cocoaView setState:state];
            }
          }
        }
      }
    }
  }
}

auto pRadioLabel::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 1, geometry.y(),
    geometry.width() + 2, geometry.height()
  });
}

auto pRadioLabel::setGroup(sGroup group) -> void {
}

auto pRadioLabel::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
