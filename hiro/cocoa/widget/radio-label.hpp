@interface CocoaRadioLabel : NSButton {
@public
  phoenix::RadioLabel* radioLabel;
}
-(id) initWith:(phoenix::RadioLabel&)radioLabel;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pRadioLabel : public pWidget {
  RadioLabel& radioLabel;
  CocoaRadioLabel* cocoaRadioLabel = nullptr;

  Size minimumSize();
  void setChecked();
  void setGeometry(Geometry geometry);
  void setGroup(const group<RadioLabel>& group);
  void setText(string text);

  pRadioLabel(RadioLabel& radioLabel) : pWidget(radioLabel), radioLabel(radioLabel) {}
  void constructor();
  void destructor();
};

}
