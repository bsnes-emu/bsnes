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
  if(selection >= 0) {
    if(auto item = tabFrame->item(selection)) {
      if(item->state.image) {
        uint iconSize = hiro::pFont::size(tabFrame->font(true), " ").height();
        sizeOfLabel.width += iconSize + 2;
      }
    }
  }
  return sizeOfLabel;
}

-(void) drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)tabRect {
  int selection = [cocoaTabFrame indexOfTabViewItem:self];
  if(selection >= 0) {
    if(auto item = tabFrame->item(selection)) {
      if(item->state.image) {
        uint iconSize = hiro::pFont::size(tabFrame->font(true), " ").height();
        NSImage* image = NSMakeImage(item->state.image);

        [[NSGraphicsContext currentContext] saveGraphicsState];
        NSRect targetRect = NSMakeRect(tabRect.origin.x, tabRect.origin.y + 2, iconSize, iconSize);
        [image drawInRect:targetRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];
        [[NSGraphicsContext currentContext] restoreGraphicsState];

        tabRect.origin.x += iconSize + 2;
        tabRect.size.width -= iconSize + 2;
      }
    }
  }
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
  @autoreleasepool {
    if(auto p = item->self()) {
      p->cocoaTabFrameItem = [[CocoaTabFrameItem alloc] initWith:self()];
      [p->cocoaTabFrameItem setLabel:[NSString stringWithUTF8String:item->state.text]];
      [cocoaView addTabViewItem:p->cocoaTabFrameItem];
    }
  }
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
  @autoreleasepool {
    if(auto p = item->self()) {
      [cocoaView removeTabViewItem:p->cocoaTabFrameItem];
    }
  }
}

auto pTabFrame::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  for(auto& item : state().items) {
    if(auto& layout = item->state.layout) {
      if(auto self = layout->self()) self->setEnabled(layout->enabled(true));
    }
  }
}

auto pTabFrame::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  for(auto& item : state().items) {
    if(auto& layout = item->state.layout) {
      if(auto self = layout->self()) self->setFont(layout->font(true));
    }
  }
}

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 7, geometry.y() - 5,
    geometry.width() + 14, geometry.height() + 6
  });
  geometry.setGeometry({
    geometry.x() + 1, geometry.y() + 22,
    geometry.width() - 2, geometry.height() - 32
  });
  for(auto& item : state().items) {
    if(auto& layout = item->state.layout) {
      layout->setGeometry(geometry);
    }
  }
  _synchronizeLayout();
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
}

auto pTabFrame::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  for(auto& item : state().items) {
    if(auto& layout = item->state.layout) {
      if(auto self = layout->self()) self->setVisible(layout->visible(true));
    }
  }
}

auto pTabFrame::_synchronizeLayout() -> void {
  @autoreleasepool {
    NSTabViewItem* tabViewItem = [cocoaView selectedTabViewItem];
    int selected = tabViewItem ? [cocoaView indexOfTabViewItem:tabViewItem] : -1;
    for(auto& item : state().items) {
      item->state.selected = item->offset() == selected;
      if(auto& layout = item->state.layout) {
        if(auto self = layout->self()) self->setVisible(layout->visible(true) && item->selected());
      }
    }
  }
}

}

#endif
