#if defined(Hiro_RadioButton)

@implementation CocoaRadioButton : NSButton

-(id) initWith:(hiro::mRadioButton&)radioButtonReference {
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
  if(!wasChecked) radioButton->doActivate();
}

@end

namespace hiro {

auto pRadioButton::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaRadioButton = [[CocoaRadioButton alloc] initWith:self()];
    pWidget::construct();

    setBordered(state().bordered);
    if(state().checked) setChecked();
    setIcon(state().icon);
    setOrientation(state().orientation);
    setText(state().text);
  }
}

auto pRadioButton::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pRadioButton::minimumSize() const -> Size {
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

auto pRadioButton::setBordered(bool bordered) -> void {
}

auto pRadioButton::setChecked() -> void {
  @autoreleasepool {
    if(auto group = state().group) {
      for(auto& weak : group->state.objects) {
        if(auto object = weak.acquire()) {
          if(auto self = object->self()) {
            if(auto p = dynamic_cast<pRadioButton*>(self)) {
              auto state = this == p ? NSOnState : NSOffState;
              [p->cocoaView setState:state];
            }
          }
        }
      }
    }
  }
}

auto pRadioButton::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y() - 2,
    geometry.width() + 4, geometry.height() + 4
  });
}

auto pRadioButton::setGroup(sGroup group) -> void {
}

auto pRadioButton::setIcon(const image& icon) -> void {
  @autoreleasepool {
    [cocoaView setImage:NSMakeImage(icon)];
  }
}

auto pRadioButton::setOrientation(Orientation orientation) -> void {
  @autoreleasepool {
    if(orientation == Orientation::Horizontal) [cocoaView setImagePosition:NSImageLeft];
    if(orientation == Orientation::Vertical  ) [cocoaView setImagePosition:NSImageAbove];
  }
}

auto pRadioButton::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
