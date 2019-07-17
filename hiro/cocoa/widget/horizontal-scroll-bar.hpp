#if defined(Hiro_HorizontalScrollBar)

@interface CocoaHorizontalScrollBar : NSScroller {
@public
  hiro::mHorizontalScrollBar* horizontalScrollBar;
}
-(id) initWith:(hiro::mHorizontalScrollBar&)horizontalScrollBar;
-(void) update;
-(IBAction) scroll:(id)sender;
@end

namespace hiro {

struct pHorizontalScrollBar : pWidget {
  Declare(HorizontalScrollBar, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(uint length) -> void;
  auto setPosition(uint position) -> void;

  CocoaHorizontalScrollBar* cocoaHorizontalScrollBar = nullptr;
};

}

#endif
