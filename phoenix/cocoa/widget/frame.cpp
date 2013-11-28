@implementation CocoaFrame : NSBox

-(id) initWith:(phoenix::Frame&)frameReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    frame = &frameReference;

    [self setTitle:@""];
  }
  return self;
}

@end

namespace phoenix {

void pFrame::setEnabled(bool enabled) {
  if(frame.state.layout) frame.state.layout->setEnabled(frame.state.layout->enabled());
  pWidget::setEnabled(enabled);
}

void pFrame::setFont(string font) {
  @autoreleasepool {
    [cocoaView setTitleFont:pFont::cocoaFont(font)];
  }
}

void pFrame::setGeometry(Geometry geometry) {
  bool empty = frame.state.text.empty();
  Size size = Font::size(frame.font(), frame.state.text);
  pWidget::setGeometry({
    geometry.x - 3, geometry.y - (empty ? size.height - 2 : 1),
    geometry.width + 6, geometry.height + (empty ? size.height + 2 : 5)
  });
  if(frame.state.layout == nullptr) return;
  geometry.x += 1, geometry.y += (empty ? 1 : size.height - 2);
  geometry.width -= 2, geometry.height -= (empty ? 1 : size.height - 1);
  frame.state.layout->setGeometry(geometry);
}

void pFrame::setText(string text) {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

void pFrame::setVisible(bool visible) {
  if(frame.state.layout) frame.state.layout->setVisible(frame.state.layout->visible());
  pWidget::setVisible(visible);
}

void pFrame::constructor() {
  @autoreleasepool {
    cocoaView = cocoaFrame = [[CocoaFrame alloc] initWith:frame];
  }
}

void pFrame::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

void pFrame::orphan() {
  destructor();
  constructor();
}

}
