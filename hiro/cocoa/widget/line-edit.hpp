#if defined(Hiro_LineEdit)

@interface CocoaLineEdit : NSTextField <NSTextFieldDelegate> {
@public
  hiro::mLineEdit* lineEdit;
}
-(id) initWith:(hiro::mLineEdit&)lineEdit;
-(void) textDidChange:(NSNotification*)n;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pLineEdit : pWidget {
  Declare(LineEdit, Widget)

  auto minimumSize() const -> Size override;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;

  CocoaLineEdit* cocoaLineEdit = nullptr;
};

}

#endif
