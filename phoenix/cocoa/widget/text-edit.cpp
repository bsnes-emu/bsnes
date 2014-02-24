@implementation CocoaTextEdit : NSScrollView

-(id) initWith:(phoenix::TextEdit&)textEditReference {
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
  if(textEdit->onChange) textEdit->onChange();
}

@end

namespace phoenix {

void pTextEdit::setBackgroundColor(Color color) {
}

void pTextEdit::setCursorPosition(unsigned position) {
  @autoreleasepool {
    string text = [[[cocoaView content] string] UTF8String];
    position = min(position, text.length());
    [[cocoaView content] setSelectedRange:NSMakeRange(position, 0)];
  }
}

void pTextEdit::setEditable(bool editable) {
  @autoreleasepool {
    [[cocoaView content] setEditable:editable];
  }
}

void pTextEdit::setFont(string font) {
  @autoreleasepool {
    [[cocoaView content] setFont:pFont::cocoaFont(font)];
  }
}

void pTextEdit::setForegroundColor(Color color) {
}

void pTextEdit::setText(string text) {
  @autoreleasepool {
    [[cocoaView content] setString:[NSString stringWithUTF8String:text]];
  }
}

void pTextEdit::setWordWrap(bool wordWrap) {
  @autoreleasepool {
    [cocoaView configure];
  }
}

string pTextEdit::text() {
  @autoreleasepool {
    return [[[cocoaView content] string] UTF8String];
  }
}

void pTextEdit::constructor() {
  @autoreleasepool {
    cocoaView = cocoaTextEdit = [[CocoaTextEdit alloc] initWith:textEdit];
    setEditable(textEdit.state.editable);
    setWordWrap(textEdit.state.wordWrap);
    setFont(textEdit.font());
    setText(textEdit.state.text);
    setCursorPosition(textEdit.state.cursorPosition);
  }
}

void pTextEdit::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
