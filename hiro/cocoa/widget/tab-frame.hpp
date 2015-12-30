#if defined(Hiro_TabFrame)

@interface CocoaTabFrame : NSTabView <NSTabViewDelegate> {
@public
  hiro::mTabFrame* tabFrame;
}
-(id) initWith:(hiro::mTabFrame&)tabFrame;
-(void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem;
@end

@interface CocoaTabFrameItem : NSTabViewItem {
@public
  hiro::mTabFrame* tabFrame;
  CocoaTabFrame* cocoaTabFrame;
}
-(id) initWith:(hiro::mTabFrame&)tabFrame;
-(NSSize) sizeOfLabel:(BOOL)shouldTruncateLabel;
-(void) drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)tabRect;
@end

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto remove(sTabFrameItem item) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setNavigation(Navigation navigation) -> void;

  auto _synchronizeLayout() -> void;
  auto _updateSelected(int selected) -> void;

  CocoaTabFrame* cocoaTabFrame = nullptr;
  vector<CocoaTabFrameItem*> tabs;
};

}

#endif
