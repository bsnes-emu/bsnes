#if defined(Hiro_TabFrame)

@implementation CocoaTabFrame : NSTabView

-(id) initWith:(hiro::mTabFrame&)tabFrameReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    tabFrame = &tabFrameReference;

    [self setDelegate:self];
  }
  return self;
}

-(void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
  tabFrame->self()->_updateSelected([tabView indexOfTabViewItem:tabViewItem]);
  tabFrame->self()->_synchronizeLayout();
  tabFrame->doChange();
}

@end

@implementation CocoaTabFrameItem : NSTabViewItem

-(id) initWith:(hiro::mTabFrame&)tabFrameReference {
  if(self = [super initWithIdentifier:nil]) {
    tabFrame = &tabFrameReference;
    cocoaTabFrame = tabFrame->self()->cocoaTabFrame;
  }
  return self;
}

-(NSSize) sizeOfLabel:(BOOL)shouldTruncateLabel {
  NSSize sizeOfLabel = [super sizeOfLabel:shouldTruncateLabel];
  int selection = [cocoaTabFrame indexOfTabViewItem:self];
  if(selection < 0) return sizeOfLabel;  //should never happen
//  if(!tabFrame->state.image[selection].empty()) {
//    uint iconSize = hiro::pFont::size(tabFrame->font(true), " ").height();
//    sizeOfLabel.width += iconSize + 2;
//  }
  return sizeOfLabel;
}

-(void) drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)tabRect {
  int selection = [cocoaTabFrame indexOfTabViewItem:self];
/*if(selection >= 0 && !tabFrame->state.image[selection].empty()) {
    uint iconSize = phoenix::Font::size(tabFrame->font(), " ").height;
    NSImage* image = NSMakeImage(tabFrame->state.image[selection]);

    [[NSGraphicsContext currentContext] saveGraphicsState];
    NSRect targetRect = NSMakeRect(tabRect.origin.x, tabRect.origin.y + 2, iconSize, iconSize);
    [image drawInRect:targetRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];
    [[NSGraphicsContext currentContext] restoreGraphicsState];

    tabRect.origin.x += iconSize + 2;
    tabRect.size.width -= iconSize + 2;
  }*/
  [super drawLabel:shouldTruncateLabel inRect:tabRect];
}

@end

namespace hiro {

auto pTabFrame::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaTabFrame = [[CocoaTabFrame alloc] initWith:self()];
    pWidget::construct();

    setNavigation(state().navigation);
  }
}

auto pTabFrame::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pTabFrame::append(sTabFrameItem item) -> void {
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
}

/*
auto pTabFrame::append(string text, const image& image) -> void {
  @autoreleasepool {
    CocoaTabFrameItem* item = [[CocoaTabFrameItem alloc] initWith:tabFrame];
    [item setLabel:[NSString stringWithUTF8String:text]];
    [cocoaView addTabViewItem:item];
    tabs.append(item);
  }
}

auto pTabFrame::remove(unsigned selection) -> void {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [cocoaView removeTabViewItem:item];
    tabs.remove(selection);
  }
}

auto pTabFrame::setEnabled(bool enabled) -> void {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}
*/

auto pTabFrame::setGeometry(Geometry geometry) -> void {
/*
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
*/
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
}

/*
auto pTabFrame::setSelection(unsigned selection) -> void {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [cocoaView selectTabViewItem:item];
  }
  synchronizeLayout();
}

auto pTabFrame::setText(unsigned selection, string text) -> void {
  @autoreleasepool {
    CocoaTabFrameItem* item = tabs[selection];
    [item setLabel:[NSString stringWithUTF8String:text]];
  }
}

auto pTabFrame::setVisible(bool visible) -> void {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}
*/

auto pTabFrame::_synchronizeLayout() -> void {
/*
  uint selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
*/
}

auto pTabFrame::_updateSelected(int selected) -> void {
}

}

#endif
