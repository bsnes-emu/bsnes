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
  if(comboButton->onChange) comboButton->onChange();
}

@end

namespace phoenix {

void pComboButton::append(const string& text) {
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

void pComboButton::modify(unsigned row, const string& text) {
  @autoreleasepool {
    [[cocoaView itemAtIndex:row] setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pComboButton::remove(unsigned row) {
  @autoreleasepool {
    [cocoaView removeItemAtIndex:row];
  }
}

void pComboButton::reset() {
  @autoreleasepool {
    [cocoaView removeAllItems];
  }
}

unsigned pComboButton::selection() {
  @autoreleasepool {
    return [cocoaView indexOfSelectedItem];
  }
}

void pComboButton::setGeometry(const Geometry& geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y,
    geometry.width + 4, geometry.height
  });
}

void pComboButton::setSelection(unsigned row) {
  @autoreleasepool {
    [cocoaView selectItemAtIndex:row];
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
