@interface CocoaCheckButton : NSButton {
@public
  phoenix::CheckButton *checkButton;
}
-(id) initWith:(phoenix::CheckButton&)checkButton;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pCheckButton : public pWidget {
  CheckButton &checkButton;
  CocoaCheckButton *cocoaCheckButton = nullptr;

  bool checked();
  Size minimumSize();
  void setChecked(bool checked);
  void setGeometry(const Geometry &geometry);
  void setText(const string &text);

  pCheckButton(CheckButton &checkButton) : pWidget(checkButton), checkButton(checkButton) {}
  void constructor();
  void destructor();
};

}
