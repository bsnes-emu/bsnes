@implementation CocoaCanvas : NSImageView

-(id) initWith :(phoenix::Canvas&)canvasReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    canvas = &canvasReference;
    [self setEditable:NO];  //disable image drag-and-drop functionality
    NSTrackingArea *area = [[[NSTrackingArea alloc] initWithRect:[self frame]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect
      owner:self userInfo:nil
    ] autorelease];
    [self addTrackingArea:area];
  }
  return self;
}

-(void) mouseButton :(NSEvent*)event down:(BOOL)isDown {
  if(auto &callback = isDown ? canvas->onMousePress : canvas->onMouseRelease) {
    switch([event buttonNumber]) {
    case 0: return callback(phoenix::Mouse::Button::Left);
    case 1: return callback(phoenix::Mouse::Button::Right);
    case 2: return callback(phoenix::Mouse::Button::Middle);
    }
  }
}

-(void) mouseExited :(NSEvent*)event {
  if(canvas->onMouseLeave) canvas->onMouseLeave();
}

-(void) mouseMove :(NSEvent*)event {
  if([event window] == nil) return;
  NSPoint location = [self convertPoint:[event locationInWindow] fromView:nil];
  if(canvas->onMouseMove) canvas->onMouseMove({location.x, [self frame].size.height - 1 - location.y});
}

-(void) mouseDown :(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) mouseUp :(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) mouseDragged :(NSEvent*)event {
  [self mouseMove:event];
}

-(void) rightMouseDown :(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) rightMouseUp :(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) rightMouseDragged :(NSEvent*)event {
  [self mouseMove:event];
}

-(void) otherMouseDown :(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) otherMouseUp :(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) otherMouseDragged :(NSEvent*)event {
  [self mouseMove:event];
}

@end

namespace phoenix {

void pCanvas::setSize(const Size &size) {
  @autoreleasepool {
    NSImage *image = [[[NSImage alloc] initWithSize:NSMakeSize(size.width, size.height)] autorelease];
    NSBitmapImageRep *bitmap = [[[NSBitmapImageRep alloc]
      initWithBitmapDataPlanes:nil
      pixelsWide:size.width pixelsHigh:size.height
      bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
      isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace
      bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
      bytesPerRow:size.width * 4 bitsPerPixel:32
    ] autorelease];

    [image addRepresentation:bitmap];
    [cocoaView setImage:image];
  }
}

void pCanvas::update() {
  @autoreleasepool {
    if(NSBitmapImageRep *bitmap = [[[cocoaView image] representations] objectAtIndex:0]) {
      uint8_t *target = [bitmap bitmapData];
      uint32_t *source = canvas.state.data;

      for(unsigned n = 0; n < canvas.state.width * canvas.state.height; n++) {
        *target++ = *source >> 16;
        *target++ = *source >>  8;
        *target++ = *source >>  0;
        *target++ = *source >> 24;
        source++;
      }

      [cocoaView setNeedsDisplay:YES];
    }
  }
}

void pCanvas::constructor() {
  @autoreleasepool {
    cocoaView = cocoaCanvas = [[CocoaCanvas alloc] initWith:canvas];
    setSize(canvas.size());
  }
}

void pCanvas::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
