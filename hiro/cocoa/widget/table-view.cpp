#if defined(Hiro_TableView)

@implementation CocoaTableView : NSScrollView

-(id) initWith:(hiro::mTableView&)tableViewReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    tableView = &tableViewReference;
    content = [[CocoaTableViewContent alloc] initWithFrame:NSMakeRect(0, 0, 0, 0)];

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

-(CocoaTableViewContent*) content {
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
  tableView->resizeColumns();
}

-(void) reloadColumns {
  while([[content tableColumns] count]) {
    [content removeTableColumn:[[content tableColumns] lastObject]];
  }

  for(auto& tableViewColumn : tableView->state.columns) {
    auto column = tableViewColumn->offset();

    NSTableColumn* tableColumn = [[NSTableColumn alloc] initWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
    NSTableHeaderCell* headerCell = [[NSTableHeaderCell alloc] initTextCell:[NSString stringWithUTF8String:tableViewColumn->state.text]];
    CocoaTableViewCell* dataCell = [[CocoaTableViewCell alloc] initWith:*tableView];

    [dataCell setEditable:NO];

    [tableColumn setResizingMask:NSTableColumnAutoresizingMask | NSTableColumnUserResizingMask];
    [tableColumn setHeaderCell:headerCell];
    [tableColumn setDataCell:dataCell];

    [content addTableColumn:tableColumn];
  }
}

-(NSInteger) numberOfRowsInTableView:(NSTableView*)table {
  return tableView->state.items.size();
}

-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if(auto tableViewItem = tableView->item(row)) {
    if(auto tableViewCell = tableViewItem->cell([[tableColumn identifier] integerValue])) {
      NSString* text = [NSString stringWithUTF8String:tableViewCell->state.text];
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
  for(auto& tableViewItem : tableView->state.items) {
    tableViewItem->state.selected = tableViewItem->offset() == [content selectedRow];
  }
  tableView->doChange();
}

-(IBAction) activate:(id)sender {
  tableView->doActivate();
}

-(IBAction) doubleAction:(id)sender {
  if([content clickedRow] >= 0) {
    [self activate:self];
  }
}

@end

@implementation CocoaTableViewContent : NSTableView

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

@implementation CocoaTableViewCell : NSCell

-(id) initWith:(hiro::mTableView&)tableViewReference {
  if(self = [super initTextCell:@""]) {
    tableView = &tableViewReference;
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
  if(auto tableViewItem = tableView->item([view rowAtPoint:frame.origin])) {
    if(auto tableViewCell = tableViewItem->cell([view columnAtPoint:frame.origin])) {
      NSColor* backgroundColor = nil;
      if([self isHighlighted]) backgroundColor = [NSColor alternateSelectedControlColor];
      else if(!tableView->enabled(true)) backgroundColor = [NSColor controlBackgroundColor];
      else if(auto color = tableViewCell->state.backgroundColor) backgroundColor = NSMakeColor(color);
      else backgroundColor = [NSColor controlBackgroundColor];

      [backgroundColor set];
      [NSBezierPath fillRect:frame];

      if(tableViewCell->state.checkable) {
        [buttonCell setHighlighted:YES];
        [buttonCell setState:(tableViewCell->state.checked ? NSOnState : NSOffState)];
        [buttonCell drawWithFrame:frame inView:view];
        frame.origin.x += frame.size.height + 2;
        frame.size.width -= frame.size.height + 2;
      }

      if(tableViewCell->state.icon) {
        NSImage* image = NSMakeImage(tableViewCell->state.icon, frame.size.height, frame.size.height);
        [[NSGraphicsContext currentContext] saveGraphicsState];
        NSRect targetRect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
        NSRect sourceRect = NSMakeRect(0, 0, [image size].width, [image size].height);
        [image drawInRect:targetRect fromRect:sourceRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];
        [[NSGraphicsContext currentContext] restoreGraphicsState];
        frame.origin.x += frame.size.height + 2;
        frame.size.width -= frame.size.height + 2;
      }

      if(tableViewCell->state.text) {
        NSMutableParagraphStyle* paragraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        paragraphStyle.alignment = NSTextAlignmentCenter;
        if(tableViewCell->state.alignment.horizontal() < 0.333) paragraphStyle.alignment = NSTextAlignmentLeft;
        if(tableViewCell->state.alignment.horizontal() > 0.666) paragraphStyle.alignment = NSTextAlignmentRight;
        NSColor* foregroundColor = nil;
        if([self isHighlighted]) foregroundColor = [NSColor alternateSelectedControlTextColor];
        else if(!tableView->enabled(true)) foregroundColor = [NSColor disabledControlTextColor];
        else if(auto color = tableViewCell->state.foregroundColor) foregroundColor = NSMakeColor(color);
        else foregroundColor = [NSColor textColor];
        NSString* text = [NSString stringWithUTF8String:tableViewCell->state.text];
        [text drawInRect:frame withAttributes:@{
          NSBackgroundColorAttributeName:backgroundColor,
          NSForegroundColorAttributeName:foregroundColor,
          NSFontAttributeName:hiro::pFont::create(tableViewCell->font(true)),
          NSParagraphStyleAttributeName:paragraphStyle
        }];
      }
    }
  }
}

//needed to trigger trackMouse events
-(NSUInteger) hitTestForEvent:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view {
  NSUInteger hitTest = [super hitTestForEvent:event inRect:frame ofView:view];
  NSPoint point = [view convertPoint:[event locationInWindow] fromView:nil];
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
        NSPoint point = [view convertPoint:[nextEvent locationInWindow] fromView:nil];
        NSRect rect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
        if(NSMouseInRect(point, rect, [view isFlipped])) {
          if(auto tableViewItem = tableView->item([view rowAtPoint:point])) {
            if(auto tableViewCell = tableViewItem->cell([view columnAtPoint:point])) {
              tableViewCell->state.checked = !tableViewCell->state.checked;
              tableView->doToggle(tableViewCell->instance);
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

auto pTableView::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaTableView = [[CocoaTableView alloc] initWith:self()];
    pWidget::construct();

    setAlignment(state().alignment);
    setBackgroundColor(state().backgroundColor);
    setBatchable(state().batchable);
    setBordered(state().bordered);
    setFont(self().font(true));
    setForegroundColor(state().foregroundColor);
    setHeadered(state().headered);
    setSortable(state().sortable);
  }
}

auto pTableView::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pTableView::append(sTableViewColumn column) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];
    resizeColumns();
  }
}

auto pTableView::append(sTableViewItem item) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pTableView::remove(sTableViewColumn column) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];
    resizeColumns();
  }
}

auto pTableView::remove(sTableViewItem item) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pTableView::resizeColumns() -> void {
  @autoreleasepool {
    vector<int> widths;
    int minimumWidth = 0;
    int expandable = 0;
    for(uint column : range(self().columnCount())) {
      int width = _width(column);
      widths.append(width);
      minimumWidth += width;
      if(state().columns[column]->expandable()) expandable++;
    }

    int maximumWidth = self().geometry().width() - 18;  //include margin for vertical scroll bar
    int expandWidth = 0;
    if(expandable && maximumWidth > minimumWidth) {
      expandWidth = (maximumWidth - minimumWidth) / expandable;
    }

    for(uint column : range(self().columnCount())) {
      if(auto self = state().columns[column]->self()) {
        int width = widths[column];
        if(self->state().expandable) width += expandWidth;
        NSTableColumn* tableColumn = [[cocoaView content] tableColumnWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
        [tableColumn setWidth:width];
      }
    }
  }
}

auto pTableView::setAlignment(Alignment alignment) -> void {
}

auto pTableView::setBackgroundColor(Color color) -> void {
}

auto pTableView::setBatchable(bool batchable) -> void {
  @autoreleasepool {
    [[cocoaView content] setAllowsMultipleSelection:(batchable ? YES : NO)];
  }
}

auto pTableView::setBordered(bool bordered) -> void {
}

auto pTableView::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  @autoreleasepool {
    [[cocoaView content] setEnabled:enabled];
  }
}

auto pTableView::setFont(const Font& font) -> void {
  @autoreleasepool {
    [cocoaView setFont:pFont::create(font)];
  }
}

auto pTableView::setForegroundColor(Color color) -> void {
}

auto pTableView::setHeadered(bool headered) -> void {
  @autoreleasepool {
    if(headered == state().headered) return;
    if(headered) {
      [[cocoaView content] setHeaderView:[[[NSTableHeaderView alloc] init] autorelease]];
    } else {
      [[cocoaView content] setHeaderView:nil];
    }
  }
}

auto pTableView::setSortable(bool sortable) -> void {
  //TODO
}

auto pTableView::_cellWidth(uint row, uint column) -> uint {
  uint width = 8;
  if(auto pTableViewItem = self().item(row)) {
    if(auto pTableViewCell = pTableViewItem->cell(column)) {
      if(pTableViewCell->state.checkable) {
        width += 24;
      }
      if(auto& icon = pTableViewCell->state.icon) {
        width += icon.width() + 2;
      }
      if(auto& text = pTableViewCell->state.text) {
        width += pFont::size(pTableViewCell->font(true), text).width();
      }
    }
  }
  return width;
}

auto pTableView::_columnWidth(uint column_) -> uint {
  uint width = 8;
  if(auto column = self().column(column_)) {
    if(auto& icon = column->state.icon) {
      width += icon.width() + 2;
    }
    if(auto& text = column->state.text) {
      width += pFont::size(column->font(true), text).width();
    }
    if(column->state.sorting != Sort::None) {
      width += 16;
    }
  }
  return width;
}

auto pTableView::_width(uint column) -> uint {
  if(auto width = self().column(column).width()) return width;
  uint width = 1;
  if(!self().column(column).visible()) return width;
  if(state().headered) width = max(width, _columnWidth(column));
  for(auto row : range(state().items.size())) {
    width = max(width, _cellWidth(row, column));
  }
  return width;
}

/*
auto pTableView::setSelected(bool selected) -> void {
  @autoreleasepool {
    if(selected == false) {
      [[cocoaView content] deselectAll:nil];
    }
  }
}

auto pTableView::setSelection(unsigned selection) -> void {
  @autoreleasepool {
    [[cocoaView content] selectRowIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(selection, 1)] byExtendingSelection:NO];
  }
}
*/

}

#endif
