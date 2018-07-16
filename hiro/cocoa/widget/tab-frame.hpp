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
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setNavigation(Navigation navigation) -> void;
  auto setVisible(bool visible) -> void override;

  auto _synchronizeSizable() -> void;

  CocoaTabFrame* cocoaTabFrame = nullptr;
  vector<CocoaTabFrameItem*> tabs;
};

}

#endif
