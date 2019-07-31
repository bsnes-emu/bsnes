#if defined(Hiro_TableView)

@class CocoaTableViewContent;

@interface CocoaTableView : NSScrollView <NSTableViewDelegate, NSTableViewDataSource> {
@public
  hiro::mTableView* tableView;
  CocoaTableViewContent* content;
  NSFont* font;
}
-(id) initWith:(hiro::mTableView&)tableView;
-(void) dealloc;
-(CocoaTableViewContent*) content;
-(NSFont*) font;
-(void) setFont:(NSFont*)font;
-(void) reloadColumns;
-(NSInteger) numberOfRowsInTableView:(NSTableView*)table;
-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(BOOL) tableView:(NSTableView*)table shouldShowCellExpansionForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(NSString*) tableView:(NSTableView*)table toolTipForCell:(NSCell*)cell rect:(NSRectPointer)rect tableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row mouseLocation:(NSPoint)mouseLocation;
-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableViewSelectionDidChange:(NSNotification*)notification;
-(IBAction) doubleAction:(id)sender;
@end

@interface CocoaTableViewContent : NSTableView {
}
-(void) keyDown:(NSEvent*)event;
@end

@interface CocoaTableViewCell : NSCell {
  hiro::mTableView* tableView;
  NSButtonCell* buttonCell;
}
-(id) initWith:(hiro::mTableView&)tableViewReference;
-(NSString*) stringValue;
-(void) drawWithFrame:(NSRect)frame inView:(NSView*)view;
-(NSUInteger) hitTestForEvent:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view;
-(BOOL) trackMouse:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view untilMouseUp:(BOOL)flag;
+(BOOL) prefersTrackingUntilMouseUp;
@end

namespace hiro {

struct pTableView : pWidget {
  Declare(TableView, Widget)

  auto append(sTableViewColumn column) -> void;
  auto append(sTableViewItem item) -> void;
  auto remove(sTableViewColumn column) -> void;
  auto remove(sTableViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setHeadered(bool headered) -> void;
  auto setSortable(bool sortable) -> void;

  auto _cellWidth(uint row, uint column) -> uint;
  auto _columnWidth(uint column) -> uint;
  auto _width(uint column) -> uint;

  CocoaTableView* cocoaTableView = nullptr;
};

}

#endif
