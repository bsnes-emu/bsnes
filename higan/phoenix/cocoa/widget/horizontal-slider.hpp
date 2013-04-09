@interface CocoaHorizontalSlider : NSSlider {
@public
  phoenix::HorizontalSlider *horizontalSlider;
}
-(id) initWith:(phoenix::HorizontalSlider&)horizontalSlider;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;
  CocoaHorizontalSlider *cocoaHorizontalSlider = nullptr;

  Size minimumSize();
  unsigned position();
  void setGeometry(const Geometry &geometry);
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
};

}
