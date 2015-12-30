#if defined(Hiro_TextEdit)

@interface CocoaTextEdit : NSScrollView <NSTextViewDelegate> {
@public
  hiro::mTextEdit* textEdit;
  NSTextView* content;
}
-(id) initWith:(hiro::mTextEdit&)textEdit;
-(NSTextView*) content;
-(void) configure;
-(void) textDidChange:(NSNotification*)notification;
@end

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto setBackgroundColor(Color color) -> void;
  auto setCursor(Cursor cursor) -> void;
  auto setEditable(bool editable) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  CocoaTextEdit* cocoaTextEdit = nullptr;
};

}

#endif
