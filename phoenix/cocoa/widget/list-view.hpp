@class CocoaListViewContent;

@interface CocoaListView : NSScrollView <NSTableViewDelegate, NSTableViewDataSource> {
@public
  phoenix::ListView* listView;
  CocoaListViewContent* content;
  NSFont* font;
}
-(id) initWith:(phoenix::ListView&)listView;
-(void) dealloc;
-(CocoaListViewContent*) content;
-(NSFont*) font;
-(void) setFont:(NSFont*)font;
-(void) reloadColumns;
-(NSInteger) numberOfRowsInTableView:(NSTableView*)table;
-(id) tableView:(NSTableView*)table objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(BOOL) tableView:(NSTableView*)table shouldShowCellExpansionForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(NSString*) tableView:(NSTableView*)table toolTipForCell:(NSCell*)cell rect:(NSRectPointer)rect tableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row mouseLocation:(NSPoint)mouseLocation;
-(void) tableView:(NSTableView*)table setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableViewSelectionDidChange:(NSNotification*)notification;
-(IBAction) activate:(id)sender;
-(IBAction) doubleAction:(id)sender;
@end

@interface CocoaListViewContent : NSTableView {
}
-(void) keyDown:(NSEvent*)event;
@end

@interface CocoaListViewCell : NSTextFieldCell {
}
-(NSString*) stringValue;
-(void) drawWithFrame:(NSRect)frame inView:(NSView*)view;
@end

namespace phoenix {

struct pListView : public pWidget {
  ListView& listView;
  CocoaListView* cocoaListView = nullptr;

  void append(const lstring& text);
  void autoSizeColumns();
  void remove(unsigned selection);
  void reset();
  void setCheckable(bool checkable);
  void setChecked(unsigned selection, bool checked);
  void setFont(string font);
  void setHeaderText(const lstring& text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned selection, unsigned position, const image& image);
  void setSelected(bool selected);
  void setSelection(unsigned selection);
  void setText(unsigned selection, unsigned position, string text);

  pListView(ListView& listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
};

}
