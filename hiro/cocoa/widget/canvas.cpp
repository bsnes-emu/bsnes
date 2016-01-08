#if defined(Hiro_Canvas)

@implementation CocoaCanvas : NSImageView

-(id) initWith:(hiro::mCanvas&)canvasReference {
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
  canvas->doDrop(paths);
  return YES;
}

-(void) mouseButton:(NSEvent*)event down:(BOOL)isDown {
  if(isDown) {
    switch([event buttonNumber]) {
    case 0: return canvas->doMousePress(hiro::Mouse::Button::Left);
    case 1: return canvas->doMousePress(hiro::Mouse::Button::Right);
    case 2: return canvas->doMousePress(hiro::Mouse::Button::Middle);
    }
  } else {
    switch([event buttonNumber]) {
    case 0: return canvas->doMouseRelease(hiro::Mouse::Button::Left);
    case 1: return canvas->doMouseRelease(hiro::Mouse::Button::Right);
    case 2: return canvas->doMouseRelease(hiro::Mouse::Button::Middle);
    }
  }
}

-(void) mouseExited:(NSEvent*)event {
  canvas->doMouseLeave();
}

-(void) mouseMove:(NSEvent*)event {
  if([event window] == nil) return;
  NSPoint location = [self convertPoint:[event locationInWindow] fromView:nil];
  canvas->doMouseMove({(int)location.x, (int)([self frame].size.height - 1 - location.y)});
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

namespace hiro {

auto pCanvas::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaCanvas = [[CocoaCanvas alloc] initWith:self()];
    pWidget::construct();

    setDroppable(state().droppable);
  }
}

auto pCanvas::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pCanvas::minimumSize() const -> Size {
  if(auto& icon = state().icon) return {(int)icon.width(), (int)icon.height()};
  return {0, 0};
}

auto pCanvas::setColor(Color color) -> void {
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  @autoreleasepool {
    if(droppable) {
      [cocoaCanvas registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];
    } else {
      [cocoaCanvas unregisterDraggedTypes];
    }
  }
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  update();
}

auto pCanvas::setGradient(Gradient gradient) -> void {
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  @autoreleasepool {
    [cocoaView setNeedsDisplay:YES];
  }
}

auto pCanvas::_rasterize() -> void {
  @autoreleasepool {
    int width = 0;
    int height = 0;

    if(auto& icon = state().icon) {
      width = icon.width();
      height = icon.height();
    } else {
      width = pSizable::state().geometry.width();
      height = pSizable::state().geometry.height();
    }
    if(width <= 0 || height <= 0) return;

    if(width != surfaceWidth || height != surfaceHeight) {
      [cocoaView setImage:nil];
      [surface release];
      surface = nullptr;
      bitmap = nullptr;
    }

    surfaceWidth = width;
    surfaceHeight = height;

    if(!surface) {
      surface = [[[NSImage alloc] initWithSize:NSMakeSize(width, height)] autorelease];
      bitmap = [[[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:nil
        pixelsWide:width pixelsHigh:height
        bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
        isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace
        bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
        bytesPerRow:(width * 4) bitsPerPixel:32
      ] autorelease];
      [surface addRepresentation:bitmap];
      [cocoaView setImage:surface];
    }

    auto target = (uint32*)[bitmap bitmapData];

    if(auto icon = state().icon) {
      icon.transform();
      memory::copy(target, icon.data(), icon.size());
    } else if(auto& gradient = state().gradient) {
      auto& colors = gradient.state.colors;
      image fill;
      fill.allocate(width, height);
      fill.gradient(colors[0].value(), colors[1].value(), colors[2].value(), colors[3].value());
      memory::copy(target, fill.data(), fill.size());
    } else {
      uint32 color = state().color.value();
      for(auto n : range(width * height)) target[n] = color;
    }
  }
}

}

#endif
