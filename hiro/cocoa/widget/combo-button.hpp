@interface CocoaComboButton : NSPopUpButton {
@public
  phoenix::ComboButton* comboButton;
}
-(id) initWith:(phoenix::ComboButton&)comboButton;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pComboButton : public pWidget {
  ComboButton& comboButton;
  CocoaComboButton* cocoaComboButton = nullptr;

  void append(string text);
  Size minimumSize();
  void remove(unsigned selection);
  void reset();
  void setGeometry(Geometry geometry);
  void setSelection(unsigned selection);
  void setText(unsigned selection, string text);

  pComboButton(ComboButton& comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
};

}
