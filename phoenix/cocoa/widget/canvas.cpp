@implementation CocoaCanvas : NSImageView

-(id) initWith:(phoenix::Canvas&)canvasReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    canvas = &canvasReference;
    [self setEditable:NO];  //disable image drag-and-drop functionality
    NSTrackingArea* area = [[[NSTrackingArea alloc] initWithRect:[self frame]
      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect
      owner:self userInfo:nil
    ] autorelease];
    [self addTrackingArea:area];
  }
  return self;
}

-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender {
  return DropPathsOperation(sender);
}

-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender {
  lstring paths = DropPaths(sender);
  if(paths.empty()) return NO;
  if(canvas->onDrop) canvas->onDrop(paths);
  return YES;
}

-(void) mouseButton:(NSEvent*)event down:(BOOL)isDown {
  if(auto& callback = isDown ? canvas->onMousePress : canvas->onMouseRelease) {
    switch([event buttonNumber]) {
    case 0: return callback(phoenix::Mouse::Button::Left);
    case 1: return callback(phoenix::Mouse::Button::Right);
    case 2: return callback(phoenix::Mouse::Button::Middle);
    }
  }
}

-(void) mouseExited:(NSEvent*)event {
  if(canvas->onMouseLeave) canvas->onMouseLeave();
}

-(void) mouseMove:(NSEvent*)event {
  if([event window] == nil) return;
  NSPoint location = [self convertPoint:[event locationInWindow] fromView:nil];
  if(canvas->onMouseMove) canvas->onMouseMove({location.x, [self frame].size.height - 1 - location.y});
}

-(void) mouseDown:(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) mouseUp:(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) mouseDragged:(NSEvent*)event {
  [self mouseMove:event];
}

-(void) rightMouseDown:(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) rightMouseUp:(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) rightMouseDragged:(NSEvent*)event {
  [self mouseMove:event];
}

-(void) otherMouseDown:(NSEvent*)event {
  [self mouseButton:event down:YES];
}

-(void) otherMouseUp:(NSEvent*)event {
  [self mouseButton:event down:NO];
}

-(void) otherMouseDragged:(NSEvent*)event {
  [self mouseMove:event];
}

@end

namespace phoenix {

void pCanvas::setDroppable(bool droppable) {
  @autoreleasepool {
    if(droppable) {
      [cocoaCanvas registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];
    } else {
      [cocoaCanvas unregisterDraggedTypes];
    }
  }
}

void pCanvas::setGeometry(Geometry geometry) {
  if(canvas.state.width == 0 || canvas.state.height == 0) rasterize();

  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width < geometry.width) {
    geometry.x += (geometry.width - width) / 2;
    geometry.width = width;
  }

  if(height < geometry.height) {
    geometry.y += (geometry.height - height) / 2;
    geometry.height = height;
  }

  pWidget::setGeometry(geometry);
}

void pCanvas::setMode(Canvas::Mode mode) {
  rasterize(), redraw();
}

void pCanvas::setSize(Size size) {
  rasterize(), redraw();
}

void pCanvas::constructor() {
  @autoreleasepool {
    cocoaView = cocoaCanvas = [[CocoaCanvas alloc] initWith:canvas];
  }
  setSize(canvas.size());
}

void pCanvas::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

void pCanvas::rasterize() {
  @autoreleasepool {
    unsigned width = canvas.state.width;
    unsigned height = canvas.state.height;
    if(width == 0) width = widget.state.geometry.width;
    if(height == 0) height = widget.state.geometry.height;

    if(width != surfaceWidth || height != surfaceHeight) {
      NSImage* image = [[[NSImage alloc] initWithSize:NSMakeSize(width, height)] autorelease];
      NSBitmapImageRep* bitmap = [[[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:nil
        pixelsWide:width pixelsHigh:height
        bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
        isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace
        bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
        bytesPerRow:width * 4 bitsPerPixel:32
      ] autorelease];

      [image addRepresentation:bitmap];
      [cocoaView setImage:image];

      surfaceWidth = width;
      surfaceHeight = height;
    }

    if(NSBitmapImageRep* bitmap = [[[cocoaView image] representations] objectAtIndex:0]) {
      uint32_t* target = (uint32_t*)[bitmap bitmapData];

      if(canvas.state.mode == Canvas::Mode::Color) {
        nall::image image;
        image.allocate(width, height);
        image.fill(canvas.state.color.argb());
        memcpy(target, image.data, image.size);
      }

      if(canvas.state.mode == Canvas::Mode::Gradient) {
        nall::image image;
        image.allocate(width, height);
        image.linearGradient(
          canvas.state.gradient[0].argb(), canvas.state.gradient[1].argb(), canvas.state.gradient[2].argb(), canvas.state.gradient[3].argb()
        );
        memcpy(target, image.data, image.size);
      }

      if(canvas.state.mode == Canvas::Mode::Image) {
        nall::image image = canvas.state.image;
        image.scale(width, height);
        image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
        memcpy(target, image.data, image.size);
      }

      if(canvas.state.mode == Canvas::Mode::Data) {
        if(width == canvas.state.width && height == canvas.state.height) {
          memcpy(target, canvas.state.data, width * height * sizeof(uint32_t));
        } else {
          memset(target, 0x00, width * height * sizeof(uint32_t));
        }
      }

     //ARGB -> ABGR transformation
      for(unsigned n = 0; n < width * height; n++) {
        uint32_t color = *target;
        color = (color & 0xff00ff00) | ((color & 0xff0000) >> 16) | ((color & 0x0000ff) << 16);
        *target++ = color;
      }
    }
  }
}

void pCanvas::redraw() {
  @autoreleasepool {
    [cocoaView setNeedsDisplay:YES];
  }
}

}
