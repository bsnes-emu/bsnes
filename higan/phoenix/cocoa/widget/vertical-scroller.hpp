@interface CocoaVerticalScroller : NSScroller {
@public
  phoenix::VerticalScroller *verticalScroller;
}
-(id) initWith :(phoenix::VerticalScroller&)verticalScroller;
@end

namespace phoenix {

struct pVerticalScroller : public pWidget {
  VerticalScroller &verticalScroller;
  CocoaVerticalScroller *cocoaVerticalScroller;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller &verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
};

}
