@interface CocoaLineEdit : NSTextField <NSTextFieldDelegate> {
@public
  phoenix::LineEdit *lineEdit;
}
-(id) initWith :(phoenix::LineEdit&)lineEdit;
-(void) textDidChange :(NSNotification*)n;
-(IBAction) activate :(id)sender;
@end

namespace phoenix {

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;
  CocoaLineEdit *cocoaLineEdit;

  Size minimumSize();
  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
};

}
