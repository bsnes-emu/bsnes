@interface CocoaComboButton : NSPopUpButton {
@public
  phoenix::ComboButton *comboButton;
}
-(id) initWith:(phoenix::ComboButton&)comboButton;
-(IBAction) activate:(id)sender;
@end

namespace phoenix {

struct pComboButton : public pWidget {
  ComboButton &comboButton;
  CocoaComboButton *cocoaComboButton = nullptr;

  void append(const string &text);
  Size minimumSize();
  void modify(unsigned row, const string &text);
  void remove(unsigned row);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboButton(ComboButton &comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
};

}
