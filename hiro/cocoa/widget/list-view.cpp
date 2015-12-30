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
    [content setAllowsMultipleSelection:NO];
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

  if(false) {  //listView->state.checkable) {
    NSTableColumn* tableColumn = [[NSTableColumn alloc] initWithIdentifier:@"check"];
    NSTableHeaderCell* headerCell = [[NSTableHeaderCell alloc] initTextCell:@""];
    NSButtonCell* dataCell = [[NSButtonCell alloc] initTextCell:@""];

    [dataCell setButtonType:NSSwitchButton];
    [dataCell setControlSize:NSSmallControlSize];
    [dataCell setRefusesFirstResponder:YES];

    [tableColumn setResizingMask:NSTableColumnNoResizing];
    [tableColumn setHeaderCell:headerCell];
    [tableColumn setDataCell:dataCell];
    [tableColumn setWidth:20.0];

    [content addTableColumn:tableColumn];
  }

  lstring headers;  // = listView->state.headerText;
  if(headers.size() == 0) headers.append("");
//[content setUsesAlternatingRowBackgroundColors:headers.size() >= 2];

  for(auto column : range(headers)) {
    NSTableColumn* tableColumn = [[NSTableColumn alloc] initWithIdentifier:[[NSNumber numberWithInteger:column] stringValue]];
    NSTableHeaderCell* headerCell = [[NSTableHeaderCell alloc] initTextCell:[NSString stringWithUTF8String:headers(column)]];
    CocoaListViewCell* dataCell = [[CocoaListViewCell alloc] initTextCell:@""];

    [dataCell setEditable:NO];

    [tableColumn setResizingMask:NSTableColumnAutoresizingMask | NSTableColumnUserResizingMask];
    [tableColumn setHeaderCell:headerCell];
    [tableColumn setDataCell:dataCell];

    [content addTableColumn:tableColumn];
  }
}

-(NSInteger) numberOfRowsInTableView:(NSTableView*)table {
  return 0;  //listView->state.text.size();
}

-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if([[tableColumn identifier] isEqualToString:@"check"]) {
    auto checked = false;  //listView->state.checked(row) ? NSOnState : NSOffState;
    return [NSNumber numberWithInteger:checked];
  }

  NSInteger column = [[tableColumn identifier] integerValue];
  uint height = [table rowHeight];

  NSString* text = nil;  //[NSString stringWithUTF8String:listView->state.text(row)(column)];
  NSImage* image = nil;  //NSMakeImage(listView->state.image(row)(column), height, height);

  if(image) return @{ @"text":text, @"image":image };
  return @{ @"text":text };
}

-(BOOL) tableView:(NSTableView*)table shouldShowCellExpansionForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  return NO;
}

-(NSString*) tableView:(NSTableView*)table toolTipForCell:(NSCell*)cell rect:(NSRectPointer)rect tableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row mouseLocation:(NSPoint)mouseLocation {
  return nil;
}

-(void) tableView:(NSTableView*)table setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if([[tableColumn identifier] isEqualToString:@"check"]) {
  //listView->state.checked(row) = [object integerValue] != NSOffState;
  //listView->doToggle(row);
  }
}

-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  [cell setFont:[self font]];
}

-(void) tableViewSelectionDidChange:(NSNotification*)notification {
//listView->state.selected = true;
//listView->state.selection = [content selectedRow];
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

@implementation CocoaListViewCell : NSTextFieldCell

//used by type-ahead
-(NSString*) stringValue {
  return [[self objectValue] objectForKey:@"text"];
}

-(void) drawWithFrame:(NSRect)frame inView:(NSView*)view {
  NSString* text = [[self objectValue] objectForKey:@"text"];
  NSImage* image = [[self objectValue] objectForKey:@"image"];
  uint textDisplacement = 0;

  if(image) {
    [[NSGraphicsContext currentContext] saveGraphicsState];

    NSRect targetRect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
    NSRect sourceRect = NSMakeRect(0, 0, [image size].width, [image size].height);
    [image drawInRect:targetRect fromRect:sourceRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];

    [[NSGraphicsContext currentContext] restoreGraphicsState];
    textDisplacement = frame.size.height + 2;
  }

  NSRect textRect = NSMakeRect(
    frame.origin.x + textDisplacement, frame.origin.y,
    frame.size.width - textDisplacement, frame.size.height
  );

  NSColor* textColor = [self isHighlighted]
  ? [NSColor alternateSelectedControlTextColor]
  : [NSColor textColor];

  [text drawInRect:textRect withAttributes:@{
    NSForegroundColorAttributeName:textColor,
    NSFontAttributeName:[self font]
  }];
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
}

auto pListView::append(sListViewItem item) -> void {
}

auto pListView::remove(sListViewHeader header) -> void {
}

auto pListView::remove(sListViewItem item) -> void {
}

auto pListView::resizeColumns() -> void {
}

auto pListView::setAlignment(Alignment alignment) -> void {
}

auto pListView::setBackgroundColor(Color color) -> void {
}

auto pListView::setBatchable(bool batchable) -> void {
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

/*
auto pListView::append(const lstring& text) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

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

auto pListView::remove(unsigned selection) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pListView::reset() -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pListView::setCheckable(bool checkable) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];
  }
}

auto pListView::setChecked(unsigned selection, bool checked) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

auto pListView::setHeaderText(const lstring& text) -> void {
  @autoreleasepool {
    [cocoaView reloadColumns];
  }
}

auto pListView::setHeaderVisible(bool visible) -> void {
  @autoreleasepool {
    if(visible) {
      [[cocoaView content] setHeaderView:[[[NSTableHeaderView alloc] init] autorelease]];
    } else {
      [[cocoaView content] setHeaderView:nil];
    }
  }
}

auto pListView::setImage(unsigned selection, unsigned position, const image& image) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
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

auto pListView::setText(unsigned selection, unsigned position, const string text) -> void {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}
*/

}

#endif
