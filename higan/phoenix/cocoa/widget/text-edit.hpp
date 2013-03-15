@interface CocoaTextEdit : NSScrollView <NSTextViewDelegate> {
@public
  phoenix::TextEdit *textEdit;
  NSTextView *content;
}
-(id) initWith :(phoenix::TextEdit&)textEdit;
-(NSTextView*) content;
-(void) configure;
-(void) textDidChange :(NSNotification*)notification;
@end

namespace phoenix {

struct pTextEdit : public pWidget {
  TextEdit &textEdit;
  CocoaTextEdit *cocoaTextEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setFont(const string &font);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
};

}
