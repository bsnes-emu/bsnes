@interface CocoaVerticalScroller : NSScroller {
@public
  phoenix::VerticalScroller* verticalScroller;
}
-(id) initWith:(phoenix::VerticalScroller&)verticalScroller;
-(void) update;
-(IBAction) scroll:(id)sender;
@end

namespace phoenix {

struct pVerticalScroller : public pWidget {
  VerticalScroller& verticalScroller;
  CocoaVerticalScroller* cocoaVerticalScroller = nullptr;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller& verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
};

}
