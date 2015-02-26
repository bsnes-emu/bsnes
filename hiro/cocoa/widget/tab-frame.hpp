@interface CocoaTabFrame : NSTabView <NSTabViewDelegate> {
@public
  phoenix::TabFrame* tabFrame;
}
-(id) initWith:(phoenix::TabFrame&)tabFrame;
-(void) tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem;
@end

@interface CocoaTabFrameItem : NSTabViewItem {
@public
  phoenix::TabFrame* tabFrame;
  CocoaTabFrame* cocoaTabFrame;
}
-(id) initWith:(phoenix::TabFrame&)tabFrame;
-(NSSize) sizeOfLabel:(BOOL)shouldTruncateLabel;
-(void) drawLabel:(BOOL)shouldTruncateLabel inRect:(NSRect)tabRect;
@end

namespace phoenix {

struct pTabFrame : public pWidget {
  TabFrame& tabFrame;
  CocoaTabFrame* cocoaTabFrame = nullptr;
  vector<CocoaTabFrameItem*> tabs;

  void append(string text, const image& image);
  void remove(unsigned selection);
  void setEnabled(bool enabled);
  void setGeometry(Geometry geometry);
  void setImage(unsigned selection, const image& image);
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);
  void setVisible(bool visible);

  pTabFrame(TabFrame& tabFrame) : pWidget(tabFrame), tabFrame(tabFrame) {}
  void constructor();
  void destructor();
  void synchronizeLayout();
};

}
