@implementation CocoaListView : NSScrollView

-(id) initWith:(phoenix::ListView&)listViewReference {
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

  unsigned fontHeight = phoenix::pFont::size(font, " ").height;
  [content setFont:font];
  [content setRowHeight:fontHeight];
  [self reloadColumns];
}

-(void) reloadColumns {
  while([[content tableColumns] count]) {
    [content removeTableColumn:[[content tableColumns] lastObject]];
  }

  if(listView->state.checkable) {
    NSTableColumn *tableColumn = [[NSTableColumn alloc] initWithIdentifier:@"check"];
    NSTableHeaderCell *headerCell = [[NSTableHeaderCell alloc] initTextCell:@""];
    NSButtonCell *dataCell = [[NSButtonCell alloc] initTextCell:@""];

    [dataCell setButtonType:NSSwitchButton];
    [dataCell setControlSize:NSSmallControlSize];
    [dataCell setRefusesFirstResponder:YES];

    [tableColumn setResizingMask:NSTableColumnNoResizing];
    [tableColumn setHeaderCell:headerCell];
    [tableColumn setDataCell:dataCell];
    [tableColumn setWidth:20.0];

    [content addTableColumn:tableColumn];
  }

  lstring headers = listView->state.headerText;
  if(headers.size() == 0) headers.append("");
  [content setUsesAlternatingRowBackgroundColors:headers.size() >= 2];

  for(unsigned column = 0; column < headers.size(); column++) {
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
  return listView->state.text.size();
}

-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if([[tableColumn identifier] isEqualToString:@"check"]) {
    auto checked = listView->state.checked(row) ? NSOnState : NSOffState;
    return [NSNumber numberWithInteger:checked];
  }

  NSInteger column = [[tableColumn identifier] integerValue];
  unsigned height = [table rowHeight];

  NSString* text = [NSString stringWithUTF8String:listView->state.text(row)(column)];
  NSImage* image = NSMakeImage(listView->state.image(row)(column), height, height);

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
    listView->state.checked(row) = [object integerValue] != NSOffState;
    if(listView->onToggle) listView->onToggle(row);
  }
}

-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  [cell setFont:[self font]];
}

-(void) tableViewSelectionDidChange:(NSNotification*)notification {
  if(listView->onChange) listView->onChange();
}

-(IBAction) activate:(id)sender {
  if(listView->onActivate) listView->onActivate();
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
  unsigned textDisplacement = 0;

  if(image) {
    NSGraphicsContext* context = [NSGraphicsContext currentContext];
    [context saveGraphicsState];

    NSRect targetRect = NSMakeRect(frame.origin.x, frame.origin.y, frame.size.height, frame.size.height);
    NSRect sourceRect = NSMakeRect(0, 0, [image size].width, [image size].height);
    [image drawInRect:targetRect fromRect:sourceRect operation:NSCompositeSourceOver fraction:1.0 respectFlipped:YES hints:nil];

    [context restoreGraphicsState];
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

namespace phoenix {

void pListView::append(const lstring& text) {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

void pListView::autoSizeColumns() {
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

bool pListView::checked(unsigned row) {
  return listView.state.checked(row);
}

void pListView::modify(unsigned row, const lstring& text) {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

void pListView::remove(unsigned row) {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

void pListView::reset() {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

bool pListView::selected() {
  @autoreleasepool {
    return [[cocoaView content] selectedRow] >= 0;
  }
}

unsigned pListView::selection() {
  if(selected() == false) return 0;

  @autoreleasepool {
    return [[cocoaView content] selectedRow];
  }
}

void pListView::setCheckable(bool checkable) {
  @autoreleasepool {
    [cocoaView reloadColumns];
  }
}

void pListView::setChecked(unsigned row, bool checked) {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

void pListView::setFont(const string& font) {
  @autoreleasepool {
    [cocoaView setFont:pFont::cocoaFont(font)];
  }
}

void pListView::setHeaderText(const lstring& text) {
  @autoreleasepool {
    [cocoaView reloadColumns];
  }
}

void pListView::setHeaderVisible(bool visible) {
  @autoreleasepool {
    if(visible) {
      [[cocoaView content] setHeaderView:[[[NSTableHeaderView alloc] init] autorelease]];
    } else {
      [[cocoaView content] setHeaderView:nil];
    }
  }
}

void pListView::setImage(unsigned row, unsigned column, const image& image) {
  @autoreleasepool {
    [[cocoaView content] reloadData];
  }
}

void pListView::setSelected(bool selected) {
  @autoreleasepool {
    if(selected == false) {
      [[cocoaView content] deselectAll:nil];
    }
  }
}

void pListView::setSelection(unsigned row) {
  @autoreleasepool {
    [[cocoaView content] selectRowIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(row, 1)] byExtendingSelection:NO];
  }
}

void pListView::constructor() {
  @autoreleasepool {
    cocoaView = cocoaListView = [[CocoaListView alloc] initWith:listView];
    setHeaderVisible(listView.state.headerVisible);
    setHeaderText(listView.state.headerText);
  }
}

void pListView::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
