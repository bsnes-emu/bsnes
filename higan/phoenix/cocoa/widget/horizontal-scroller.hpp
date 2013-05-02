@interface CocoaHorizontalScroller : NSScroller {
@public
  phoenix::HorizontalScroller* horizontalScroller;
}
-(id) initWith:(phoenix::HorizontalScroller&)horizontalScroller;
-(void) update;
-(IBAction) scroll:(id)sender;
@end

namespace phoenix {

struct pHorizontalScroller : public pWidget {
  HorizontalScroller& horizontalScroller;
  CocoaHorizontalScroller* cocoaHorizontalScroller = nullptr;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScroller(HorizontalScroller& horizontalScroller) : pWidget(horizontalScroller), horizontalScroller(horizontalScroller) {}
  void constructor();
  void destructor();
};

}
