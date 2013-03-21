@implementation CocoaLineEdit : NSTextField

-(id) initWith:(phoenix::LineEdit&)lineEditReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    lineEdit = &lineEditReference;

    [self setDelegate:self];
    [self setTarget:self];
    [self setAction:@selector(activate:)];

    //prevent focus changes from generating activate event
    [[self cell] setSendsActionOnEndEditing:NO];
  }
  return self;
}

-(void) textDidChange:(NSNotification*)n {
  if(lineEdit->onChange) lineEdit->onChange();
}

-(IBAction) activate:(id)sender {
  if(lineEdit->onActivate) lineEdit->onActivate();
}

@end

namespace phoenix {

Size pLineEdit::minimumSize() {
  Size size = Font::size(lineEdit.font(), lineEdit.state.text);
  return {size.width + 10, size.height + 8};
}

void pLineEdit::setEditable(bool editable) {
  @autoreleasepool {
    [cocoaView setEditable:editable];
  }
}

void pLineEdit::setText(const string &text) {
  @autoreleasepool {
    [cocoaView setStringValue:[NSString stringWithUTF8String:text]];
  }
}

string pLineEdit::text() {
  @autoreleasepool {
    return [[cocoaView stringValue] UTF8String];
  }
}

void pLineEdit::constructor() {
  @autoreleasepool {
    cocoaView = cocoaLineEdit = [[CocoaLineEdit alloc] initWith:lineEdit];
    setEditable(lineEdit.state.editable);
  }
}

void pLineEdit::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
