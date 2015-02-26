@implementation CocoaTabFrame : NSTabView

-(id) initWith:(phoenix::TabFrame&)tabFrameReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    tabFrame = &tabFrameReference;

    [self setDelegate:self];
  }
  return self;
}

-(void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
  tabFrame->state.selection = [tabView indexOfTabViewItem:tabViewItem];
  tabFrame->p.synchronizeLayout();
  if(tabFrame->onChange) tabFrame->onChange();
}

@end

@implementation CocoaTabFrameItem : NSTabViewItem

-(id) initWith:(phoenix::TabFrame&)tabFrameReference {
  if(self = [super initWithIdentifier:nil]) {
    tabFrame = &tabFrameReference;
    cocoaTabFrame = tabFrame->p.cocoaTabFrame;
  }
  return self;
}

-(NSSize) sizeOfLabel:(BOOL)shouldTruncateLabel {
  NSSize sizeOfLabel = [super sizeOfLabel:shouldTruncateLabel];
  signed selection = [cocoaTabFrame indexOfTabViewItem:self];
  if(selection < 0) return sizeOfLabel;  //should never happen
  if(tabFrame->state.image[selection].empty() == false) {
    unsigned iconSize = phoenix::Font::size(tabFrame->font(), " ").height;
    sizeOfLabel.width += iconSize + 2;
  }
  return sizeOfLabel;
}

-(void) drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)tabRect {
  signed selection = [cocoaTabFrame indexOfTabViewItem:self];
  if(selection >= 0 && tabFrame->state.image[selection].empty() == false) {
    unsigned iconSize = phoenix::Font::size(tabFrame->font(), " ").height;
    NSImage* image = NSMakeImage(tabFrame->state.image[selection]);

    [[NSGraphicsContext currentContext] saveGraphicsState];
    NSRect targetRect = NSMakeRect(tabRect.origin.x, tabRect.origin.y + 2, iconSize, iconSize);
    [image drawInRect:targetRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];
    [[NSGraphicsContext currentContext] restoreGraphicsState];

    tabRect.origin.x += iconSize + 2;
    tabRect.size.width -= iconSize + 2;
  }
  [super drawLabel:shouldTruncateLabel inRect:tabRect];
}

@end

namespace phoenix {

void pTabFrame::append(string text, const image& image) {
  @autoreleasepool {
    CocoaTabFrameItem* item = [[CocoaTabFrameItem alloc] initWith:tabFrame];
    [item setLabel:[NSString stringWithUTF8String:text]];
    [cocoaView addTabViewItem:item];
    tabs.append(item);
  }
}

void pTabFrame::remove(unsigned selection) {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [cocoaView removeTabViewItem:item];
    tabs.remove(selection);
  }
}

void pTabFrame::setEnabled(bool enabled) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 7, geometry.y - 5,
    geometry.width + 14, geometry.height + 6
  });
  geometry.x += 1, geometry.width -= 2;
  geometry.y += 22, geometry.height -= 32;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == nullptr) continue;
    layout->setGeometry(geometry);
  }
  synchronizeLayout();
}

void pTabFrame::setImage(unsigned selection, const image& image) {
}

void pTabFrame::setSelection(unsigned selection) {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [cocoaView selectTabViewItem:item];
  }
  synchronizeLayout();
}

void pTabFrame::setText(unsigned selection, string text) {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [item setLabel:[NSString stringWithUTF8String:text]];
  }
}

void pTabFrame::setVisible(bool visible) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}

void pTabFrame::constructor() {
  @autoreleasepool {
    cocoaView = cocoaTabFrame = [[CocoaTabFrame alloc] initWith:tabFrame];
  }
}

void pTabFrame::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

void pTabFrame::synchronizeLayout() {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}

}
