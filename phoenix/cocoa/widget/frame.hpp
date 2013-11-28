@interface CocoaFrame : NSBox {
@public
  phoenix::Frame* frame;
}
-(id) initWith:(phoenix::Frame&)frame;
@end

namespace phoenix {

struct pFrame : public pWidget {
  Frame& frame;
  CocoaFrame* cocoaFrame = nullptr;

  void setEnabled(bool enabled);
  void setFont(string font);
  void setGeometry(Geometry geometry);
  void setText(string text);
  void setVisible(bool visible);

  pFrame(Frame& frame) : pWidget(frame), frame(frame) {}
  void constructor();
  void destructor();
  void orphan();
};

}
