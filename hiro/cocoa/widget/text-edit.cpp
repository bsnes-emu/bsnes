#if defined(Hiro_TextEdit)

@implementation CocoaTextEdit : NSScrollView

-(id) initWith:(hiro::mTextEdit&)textEditReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    textEdit = &textEditReference;

    content = [[[NSTextView alloc] initWithFrame:NSMakeRect(0, 0, 0, 0)] autorelease];
    [content setDelegate:self];
    [content setRichText:NO];

    [self setBorderType:NSBezelBorder];
    [self setDocumentView:content];
    [self configure];
  }
  return self;
}

-(NSTextView*) content {
  return content;
}

-(void) configure {
  [content setMinSize:NSMakeSize(0, 0)];
  [content setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];

  [[content textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
  [[content textContainer] setWidthTracksTextView:textEdit->wordWrap()];

  [content setHorizontallyResizable:YES];
  [content setVerticallyResizable:YES];
  [content setAutoresizingMask:NSViewNotSizable];

  [self setHasHorizontalScroller:!textEdit->wordWrap()];
  [self setHasVerticalScroller:YES];
}

-(void) textDidChange:(NSNotification*)notification {
  textEdit->state.text = [[content string] UTF8String];
  textEdit->doChange();
}

@end

namespace hiro {

auto pTextEdit::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaTextEdit = [[CocoaTextEdit alloc] initWith:self()];
    pWidget::construct();

    setEditable(state().editable);
    setWordWrap(state().wordWrap);
    setText(state().text);
    setTextCursor(state().textCursor);
  }
}

auto pTextEdit::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pTextEdit::setBackgroundColor(Color color) -> void {
}

auto pTextEdit::setEditable(bool editable) -> void {
  @autoreleasepool {
    [[cocoaView content] setEditable:(editable && self().enabled(true))];
  }
}

auto pTextEdit::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  setEditable(state().editable);  //Cocoa lacks NSTextView::setEnabled; simulate via setEnabled()
}

auto pTextEdit::setFont(const Font& font) -> void {
  @autoreleasepool {
    [[cocoaView content] setFont:pFont::create(font)];
  }
}

auto pTextEdit::setForegroundColor(Color color) -> void {
}

auto pTextEdit::setText(const string& text) -> void {
  @autoreleasepool {
    [[cocoaView content] setString:[NSString stringWithUTF8String:text]];
  }
}

auto pTextEdit::setTextCursor(TextCursor cursor) -> void {
  @autoreleasepool {
    //todo: handle text selection (cursor.length())
    string text = [[[cocoaView content] string] UTF8String];
    auto offset = min(cursor.offset(), text.length());
    [[cocoaView content] setSelectedRange:NSMakeRange(offset, 0)];
  }
}

auto pTextEdit::setWordWrap(bool wordWrap) -> void {
  @autoreleasepool {
    [cocoaView configure];
  }
}

auto pTextEdit::text() const -> string {
  @autoreleasepool {
    return [[[cocoaView content] string] UTF8String];
  }
}

auto pTextEdit::textCursor() const -> TextCursor {
  //TODO
  return state().textCursor;
}

}

#endif
