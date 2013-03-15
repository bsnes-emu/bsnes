@class CocoaListViewContent;

@interface CocoaListView : NSScrollView <NSTableViewDelegate, NSTableViewDataSource> {
@public
  phoenix::ListView *listView;
  CocoaListViewContent *content;
  NSFont *font;
}
-(id) initWith :(phoenix::ListView&)listView;
-(void) dealloc;
-(CocoaListViewContent*) content;
-(NSFont*) font;
-(void) setFont :(NSFont*)font;
-(void) reloadColumns;
-(NSInteger) numberOfRowsInTableView :(NSTableView*)table;
-(id) tableView :(NSTableView*)table objectValueForTableColumn :(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableView :(NSTableView*)table setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableView :(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row;
-(void) tableViewSelectionDidChange :(NSNotification*)notification;
-(IBAction) activate :(id)sender;
@end

@interface CocoaListViewContent : NSTableView {
}
-(void) keyDown :(NSEvent*)event;
@end

@interface CocoaListViewCell : NSTextFieldCell {
}
-(NSString*) stringValue;
-(void) drawWithFrame :(NSRect)frame inView:(NSView*)view;
@end

namespace phoenix {

struct pListView : public pWidget {
  ListView &listView;
  CocoaListView *cocoaListView;

  void append(const lstring &text);
  void autoSizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring &text);
  void remove(unsigned row);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setFont(const string &font);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned row, unsigned column, const image &image);
  void setSelected(bool selected);
  void setSelection(unsigned row);

  pListView(ListView &listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
};

}
