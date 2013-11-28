@interface CocoaRadioButton : NSButton {
@public
  phoenix::RadioButton* radioButton;
}
-(id) initWith:(phoenix::RadioButton&)radioButton;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pRadioButton : public pWidget {
  RadioButton& radioButton;
  CocoaRadioButton* cocoaRadioButton = nullptr;

  Size minimumSize();
  void setChecked();
  void setGeometry(Geometry geometry);
  void setGroup(const group<RadioButton>& group);
  void setImage(const image& image, Orientation orientation);
  void setText(string text);

  pRadioButton(RadioButton& radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  void constructor();
  void destructor();
};

}
