@implementation CocoaComboButton : NSPopUpButton

-(id) initWith:(phoenix::ComboButton&)comboButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0) pullsDown:NO]) {
    comboButton = &comboButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  comboButton->state.selection = [self indexOfSelectedItem];
  if(comboButton->onChange) comboButton->onChange();
}

@end

namespace phoenix {

void pComboButton::append(string text) {
  @autoreleasepool {
    [cocoaView addItemWithTitle:[NSString stringWithUTF8String:text]];
  }
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto& text : comboButton.state.text) maximumWidth = max(maximumWidth, Font::size(comboButton.font(), text).width);
  Size size = Font::size(comboButton.font(), " ");
  return {maximumWidth + 36, size.height + 6};
}

void pComboButton::remove(unsigned selection) {
  @autoreleasepool {
    [cocoaView removeItemAtIndex:selection];
  }
}

void pComboButton::reset() {
  @autoreleasepool {
    [cocoaView removeAllItems];
  }
}

void pComboButton::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y,
    geometry.width + 4, geometry.height
  });
}

void pComboButton::setSelection(unsigned selection) {
  @autoreleasepool {
    [cocoaView selectItemAtIndex:selection];
  }
}

void pComboButton::setText(unsigned selection, string text) {
  @autoreleasepool {
    [[cocoaView itemAtIndex:selection] setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pComboButton::constructor() {
  @autoreleasepool {
    cocoaView = cocoaComboButton = [[CocoaComboButton alloc] initWith:comboButton];
  }
}

void pComboButton::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
