#if defined(Hiro_ListView)

@class CocoaListViewContent;

@interface CocoaListView : NSScrollView <NSTableViewDelegate, NSTableViewDataSource> {
@public
  hiro::mListView* listView;
  CocoaListViewContent* content;
  NSFont* font;
}
-(id) initWith:(hiro::mListView&)listView;
-(void) dealloc;
-(CocoaListViewContent*) content;
-(NSFont*) font;
-(void) setFont:(NSFont*)font;
-(void) reloadColumns;
-(NSInteger) numberOfRowsInTableView:(NSTableView*)table;
-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(BOOL) tableView:(NSTableView*)table shouldShowCellExpansionForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(NSString*) tableView:(NSTableView*)table toolTipForCell:(NSCell*)cell rect:(NSRectPointer)rect tableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row mouseLocation:(NSPoint)mouseLocation;
-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableViewSelectionDidChange:(NSNotification*)notification;
-(IBAction) activate:(id)sender;
-(IBAction) doubleAction:(id)sender;
@end

@interface CocoaListViewContent : NSTableView {
}
-(void) keyDown:(NSEvent*)event;
@end

@interface CocoaListViewCell : NSCell {
  hiro::mListView* listView;
  NSButtonCell* buttonCell;
}
-(id) initWith:(hiro::mListView&)listViewReference;
-(NSString*) stringValue;
-(void) drawWithFrame:(NSRect)frame inView:(NSView*)view;
-(NSUInteger) hitTestForEvent:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view;
-(BOOL) trackMouse:(NSEvent*)event inRect:(NSRect)frame ofView:(NSView*)view untilMouseUp:(BOOL)flag;
+(BOOL) prefersTrackingUntilMouseUp;
@end

namespace hiro {

struct pListView : pWidget {
  Declare(ListView, Widget)

  auto append(sListViewHeader header) -> void;
  auto append(sListViewItem item) -> void;
  auto remove(sListViewHeader header) -> void;
  auto remove(sListViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;

  auto _cellWidth(uint row, uint column) -> uint;
  auto _columnWidth(uint column) -> uint;
  auto _width(uint column) -> uint;

  CocoaListView* cocoaListView = nullptr;
};

}

#endif
