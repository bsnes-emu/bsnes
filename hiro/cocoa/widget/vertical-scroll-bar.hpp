#if defined(Hiro_VerticalScrollBar)

@interface CocoaVerticalScrollBar : NSScroller {
@public
  hiro::mVerticalScrollBar* verticalScrollBar;
}
-(id) initWith:(hiro::mVerticalScrollBar&)verticalScrollBar;
-(void) update;
-(IBAction) scroll:(id)sender;
@end

namespace hiro {

struct pVerticalScrollBar : pWidget {
  Declare(VerticalScrollBar, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(uint length) -> void;
  auto setPosition(uint position) -> void;

  CocoaVerticalScrollBar* cocoaVerticalScrollBar = nullptr;
};

}

#endif
