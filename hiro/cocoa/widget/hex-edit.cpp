#if defined(Hiro_HexEdit)

@implementation CocoaHexEdit : NSScrollView

-(id) initWith:(phoenix::HexEdit&)hexEditReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    hexEdit = &hexEditReference;
  }
  return self;
}

@end

namespace hiro {

void pHexEdit::setBackgroundColor(Color color) {
}

void pHexEdit::setColumns(unsigned columns) {
}

void pHexEdit::setForegroundColor(Color color) {
}

void pHexEdit::setLength(unsigned length) {
}

void pHexEdit::setOffset(unsigned offset) {
}

void pHexEdit::setRows(unsigned rows) {
}

void pHexEdit::update() {
}

void pHexEdit::constructor() {
  @autoreleasepool {
    cocoaView = cocoaHexEdit = [[CocoaHexEdit alloc] initWith:hexEdit];
  }
}

void pHexEdit::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}

#endif
