@interface CocoaVerticalSlider : NSSlider {
@public
  phoenix::VerticalSlider* verticalSlider;
}
-(id) initWith:(phoenix::VerticalSlider&)verticalSlider;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pVerticalSlider : public pWidget {
  VerticalSlider& verticalSlider;
  CocoaVerticalSlider* cocoaVerticalSlider = nullptr;

  Size minimumSize();
  void setGeometry(Geometry geometry);
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider& verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
};

}
