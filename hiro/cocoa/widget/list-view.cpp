#if defined(Hiro_ListView)

@implementation CocoaListView : NSScrollView

-(id) initWith:(hiro::mListView&)listViewReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    listView = &listViewReference;
    content = [[CocoaListViewContent alloc] initWithFrame:NSMakeRect(0, 0, 0, 0)];

    [self setDocumentView:content];
    [self setBorderType:NSBezelBorder];
    [self setHasVerticalScroller:YES];

    [content setDataSource:self];
    [content setDelegate:self];
    [content setTarget:self];
    [content setDoubleAction:@selector(doubleAction:)];

    [content setAllowsColumnReordering:NO];
    [content setAllowsColumnResizing:YES];
    [content setAllowsColumnSelection:NO];
    [content setAllowsEmptySelection:YES];
    [content setColumnAutoresizingStyle:NSTableViewLastColumnOnlyAutoresizingStyle];

    font = nil;
    [self setFont:nil];
  }
  return self;
}

-(void) dealloc {
  [content release];
  [font release];
  [super dealloc];
}

-(CocoaListViewContent*) content {
  return content;
}

-(NSFont*) font {
  return font;
}

-(void) setFont:(NSFont*)fontPointer {
  if(!fontPointer) fontPointer = [NSFont systemFontOfSize:12];
  [fontPointer retain];
  if(font) [font release];
  font = fontPointer;

  uint fontHeight = hiro::pFont::size(font, " ").height();
  [content setFont:font];
  [content setRowHeight:fontHeight];
  [self reloadColumns];
}

-(void) reloadColumns {
  while([[content tableColumns] count]) {
    [content removeTableColumn:[[content tableColumns] lastObject]];
  }

  if(auto listViewHeader = listView->state.header) {
    for(auto& listViewColumn : listViewHeader->state.columns) {
      auto column = listViewColumn->offset();

      NSTableColumn* tableColumn = [[NSTableColumn alloc] initWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
      NSTableHeaderCell* headerCell = [[NSTableHeaderCell alloc] initTextCell:[NSString stringWithUTF8String:listViewColumn->state.text]];
      CocoaListViewCell* dataCell = [[CocoaListViewCell alloc] initWith:*listView];

      [dataCell setEditable:NO];

      [tableColumn setResizingMask:NSTableColumnAutoresizingMask | NSTableColumnUserResizingMask];
      [tableColumn setHeaderCell:headerCell];
      [tableColumn setDataCell:dataCell];

      [content addTableColumn:tableColumn];
    }
  }
}

-(NSInteger) numberOfRowsInTableView:(NSTableView*)table {
  return listView->state.items.size();
}

-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if(auto listViewItem = listView->item(row)) {
    if(auto listViewCell = listViewItem->cell([[tableColumn identifier] integerValue])) {
      NSString* text = [NSString stringWithUTF8String:listViewCell->state.text];
      return @{ @"text":text };  //used by type-ahead
    }
  }
  return @{};
}

-(BOOL) tableView:(NSTableView*)table shouldShowCellExpansionForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  return NO;
}

-(NSString*) tableView:(NSTableView*)table toolTipForCell:(NSCell*)cell rect:(NSRectPointer)rect tableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row mouseLocation:(NSPoint)mouseLocation {
  return nil;
}

-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  [cell setFont:[self font]];
}

-(void) tableViewSelectionDidChange:(NSNotification*)notification {
  for(auto& listViewItem : listView->state.items) {
    listViewItem->state.selected = listViewItem->offset() == [content selectedRow];
  }
  listView->doChange();
}

-(IBAction) activate:(id)sender {
  listView->doActivate();
}

-(IBAction) doubleAction:(id)sender {
  if([content clickedRow] >= 0) {
    [self activate:self];
  }
}

@end

@implementation CocoaListViewContent : NSTableView

-(void) keyDown:(NSEvent*)event {
  auto character = [[event characters] characterAtIndex:0];
  if(character == NSEnterCharacter || character == NSCarriageReturnCharacter) {
    if([self selectedRow] >= 0) {
      [[self delegate] activate:self];
      return;
    }
  }

  [super keyDown:event];
}

@end

@implementation CocoaListViewCell : NSCell

-(id) initWith:(hiro::mListView&)listViewReference {
  if(self = [super initTextCell:@""]) {
    listView = &listViewReference;
    buttonCell = [[NSButtonCell alloc] initTextCell:@""];
    [buttonCell setButtonType:NSSwitchButton];
    [buttonCell setControlSize:NSSmallControlSize];
    [buttonCell setRefusesFirstResponder:YES];
    [buttonCell setTarget:self];
  }
  return self;
}

//used by type-ahead
-(NSString*) stringValue {
  return [[self objectValue] objectForKey:@"text"];
}

-(void) drawWithFrame:(NSRect)frame inView:(NSView*)view {
  if(auto listViewItem = listView->item([view rowAtPoint:frame.origin])) {
    if(auto listViewCell = listViewItem->cell([view columnAtPoint:frame.origin])) {
      NSColor* backgroundColor = nil;
      if([self isHighlighted]) backgroundColor = [NSColor alternateSelectedControlColor];
      else if(auto color = listViewCell->state.backgroundColor) backgroundColor = NSMakeColor(color);
      else backgroundColor = [NSColor controlBackgroundColor];

      [backgroundColor set];
      [NSBezierPath fillRect:frame];

      if(listViewCell->state.checkable) {
        [buttonCell setHighlighted:YES];
        [buttonCell setState:(listViewCell->state.checked ? NSOnState : NSOffState)];
        [buttonCell drawWithFrame:frame inView:view];
        frame.origin.x += frame.size.height + 2;
        frame.size.width -= frame.size.height + 2;
      }

      if(listViewCell->state.image) {
        NSImage* image = NSMakeImage(listViewCell->state.image, frame.size.height, frame.size.height);
        [[NSGraphicsContext currentContext] saveGraphicsState];
        NSRect targetRect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
        NSRect sourceRect = NSMakeRect(0, 0, [image size].width, [image size].height);
        [image drawInRect:targetRect fromRect:sourceRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];
        [[NSGraphicsContext currentContext] restoreGraphicsState];
        frame.origin.x += frame.size.height + 2;
        frame.size.width -= frame.size.height + 2;
      }

      if(listViewCell->state.text) {
        NSMutableParagraphStyle* paragraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        paragraphStyle.alignment = NSTextAlignmentCenter;
        if(listViewCell->state.alignment.horizontal() < 0.333) paragraphStyle.alignment = NSTextAlignmentLeft;
        if(listViewCell->state.alignment.horizontal() > 0.666) paragraphStyle.alignment = NSTextAlignmentRight;
        NSColor* foregroundColor = nil;
        if([self isHighlighted]) foregroundColor = [NSColor alternateSelectedControlTextColor];
        else if(auto color = listViewCell->state.foregroundColor) foregroundColor = NSMakeColor(color);
        else foregroundColor = [NSColor textColor];
        NSString* text = [NSString stringWithUTF8String:listViewCell->state.text];
        [text drawInRect:frame withAttributes:@{
          NSBackgroundColorAttributeName:backgroundColor,
          NSForegroundColorAttributeName:foregroundColor,
          NSFontAttributeName:hiro::pFont::create(listViewCell->font(true)),
          NSParagraphStyleAttributeName:paragraphStyle
        }];
      }
    }
  }
}

//needed to trigger trackMouse events
-(NSUInteger) hitTestForEvent:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view {
  NSUInteger hitTest = [super hitTestForEvent:event inRect:frame ofView:view];
  NSPoint point = [view convertPointFromBase:[event locationInWindow]];
  NSRect rect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
  if(NSMouseInRect(point, rect, [view isFlipped])) {
    hitTest |= NSCellHitTrackableArea;
  }
  return hitTest;
}

//I am unable to get startTrackingAt:, continueTracking:, stopTracking: to work
//so instead, I have to run a modal loop on events until the mouse button is released
-(BOOL) trackMouse:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view untilMouseUp:(BOOL)flag {
  if([event type] == NSLeftMouseDown) {
    NSWindow* window = [view window];
    NSEvent* nextEvent;
    while((nextEvent = [window nextEventMatchingMask:(NSLeftMouseDragged | NSLeftMouseUp)])) {
      if([nextEvent type] == NSLeftMouseUp) {
        NSPoint point = [view convertPointFromBase:[nextEvent locationInWindow]];
        NSRect rect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
        if(NSMouseInRect(point, rect, [view isFlipped])) {
          if(auto listViewItem = listView->item([view rowAtPoint:point])) {
            if(auto listViewCell = listViewItem->cell([view columnAtPoint:point])) {
              listViewCell->state.checked = !listViewCell->state.checked;
              listView->doToggle(listViewCell->instance);
            }
          }
        }
        break;
      }
    }
  }
  return YES;
}

+(BOOL) prefersTrackingUntilMouseUp {
  return YES;
}

@end

namespace hiro {

auto pListView::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaListView = [[CocoaListView alloc] initWith:self()];
    pWidget::construct();

    setAlignment(state().alignment);
    setBackgroundColor(state().backgroundColor);
    setBatchable(state().batchable);
    setBordered(state().bordered);
    setFont(self().font(true));
    setForegroundColor(state().foregroundColor);
  }
}

auto pListView::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pListView::append(sListViewHeader header) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];

    header->setVisible(header->visible());
  }
}

auto pListView::append(sListViewItem item) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pListView::remove(sListViewHeader header) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];
  }
}

auto pListView::remove(sListViewItem item) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pListView::resizeColumns() -> void {
  @autoreleasepool {
    if(auto& header = state().header) {
      vector<int> widths;
      int minimumWidth = 0;
      int expandable = 0;
      for(auto column : range(header->columnCount())) {
        int width = _width(column);
        widths.append(width);
        minimumWidth += width;
        if(header->column(column).expandable()) expandable++;
      }

      int maximumWidth = self().geometry().width() - 18;  //include margin for vertical scroll bar
      int expandWidth = 0;
      if(expandable && maximumWidth > minimumWidth) {
        expandWidth = (maximumWidth - minimumWidth) / expandable;
      }

      for(auto column : range(header->columnCount())) {
        if(auto self = header->state.columns[column]->self()) {
          int width = widths[column];
          if(self->state().expandable) width += expandWidth;
          NSTableColumn* tableColumn = [[cocoaView content] tableColumnWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
          [tableColumn setWidth:width];
        }
      }
    }
  }
}

auto pListView::setAlignment(Alignment alignment) -> void {
}

auto pListView::setBackgroundColor(Color color) -> void {
}

auto pListView::setBatchable(bool batchable) -> void {
  @autoreleasepool {
    [[cocoaView content] setAllowsMultipleSelection:(batchable ? YES : NO)];
  }
}

auto pListView::setBordered(bool bordered) -> void {
}

auto pListView::setFont(const Font& font) -> void {
  @autoreleasepool {
    [cocoaView setFont:pFont::create(font)];
  }
}

auto pListView::setForegroundColor(Color color) -> void {
}

auto pListView::_cellWidth(uint row, uint column) -> uint {
  uint width = 8;
  if(auto pListViewItem = self().item(row)) {
    if(auto pListViewCell = pListViewItem->cell(column)) {
      if(pListViewCell->state.checkable) {
        width += 24;
      }
      if(auto& image = pListViewCell->state.image) {
        width += image.width() + 2;
      }
      if(auto& text = pListViewCell->state.text) {
        width += pFont::size(pListViewCell->font(true), text).width();
      }
    }
  }
  return width;
}

auto pListView::_columnWidth(uint column) -> uint {
  uint width = 8;
  if(auto& header = state().header) {
    if(auto pListViewColumn = header->column(column)) {
      if(auto& image = pListViewColumn->state.image) {
        width += image.width() + 2;
      }
      if(auto& text = pListViewColumn->state.text) {
        width += pFont::size(pListViewColumn->font(true), text).width();
      }
    }
  }
  return width;
}

auto pListView::_width(uint column) -> uint {
  if(auto& header = state().header) {
    if(auto width = header->column(column).width()) return width;
    uint width = 1;
    if(!header->column(column).visible()) return width;
    if(header->visible()) width = max(width, _columnWidth(column));
    for(auto row : range(state().items)) {
      width = max(width, _cellWidth(row, column));
    }
    return width;
  }
  return 1;
}

/*
auto pListView::autoSizeColumns() -> void {
  @autoreleasepool {
    if(listView.state.checkable) {
      NSTableColumn* tableColumn = [[cocoaView content] tableColumnWithIdentifier:@"check"];
      [tableColumn setWidth:20.0];
    }

    unsigned height = [[cocoaView content] rowHeight];
    for(unsigned column = 0; column < max(1u, listView.state.headerText.size()); column++) {
      NSTableColumn* tableColumn = [[cocoaView content] tableColumnWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
      unsigned minimumWidth = pFont::size([[tableColumn headerCell] font], listView.state.headerText(column)).width + 4;
      for(unsigned row = 0; row < listView.state.text.size(); row++) {
        unsigned width = pFont::size([cocoaView font], listView.state.text(row)(column)).width + 2;
        if(listView.state.image(row)(height).empty() == false) width += height + 2;
        if(width > minimumWidth) minimumWidth = width;
      }
      [tableColumn setWidth:minimumWidth];
    }

    [[cocoaView content] sizeLastColumnToFit];
  }
}

auto pListView::setSelected(bool selected) -> void {
  @autoreleasepool {
    if(selected == false) {
      [[cocoaView content] deselectAll:nil];
    }
  }
}

auto pListView::setSelection(unsigned selection) -> void {
  @autoreleasepool {
    [[cocoaView content] selectRowIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(selection, 1)] byExtendingSelection:NO];
  }
}
*/

}

#endif
