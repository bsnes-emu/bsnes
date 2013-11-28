@interface CocoaCheckLabel : NSButton {
@public
  phoenix::CheckLabel* checkLabel;
}
-(id) initWith:(phoenix::CheckLabel&)checkLabel;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pCheckLabel : public pWidget {
  CheckLabel& checkLabel;
  CocoaCheckLabel* cocoaCheckLabel = nullptr;

  Size minimumSize();
  void setChecked(bool checked);
  void setGeometry(Geometry geometry);
  void setText(string text);

  pCheckLabel(CheckLabel& checkLabel) : pWidget(checkLabel), checkLabel(checkLabel) {}
  void constructor();
  void destructor();
};

}
