@interface CocoaRadioButton : NSButton {
@public
  phoenix::RadioButton *radioButton;
}
-(id) initWith :(phoenix::RadioButton&)radioButton;
@end

namespace phoenix {

struct pRadioButton : public pWidget {
  RadioButton &radioButton;
  CocoaRadioButton *cocoaRadioButton;

  bool checked();
  Size minimumSize();
  void setChecked();
  void setGroup(const set<RadioButton&> &group);
  void setText(const string &text);

  pRadioButton(RadioButton &radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  void constructor();
  void destructor();
};

}
