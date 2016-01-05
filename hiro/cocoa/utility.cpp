auto NSMakeColor(const hiro::Color& color) -> NSColor* {
  return [NSColor colorWithRed:(color.red() / 255.0) green:(color.green() / 255.0) blue:(color.blue() / 255.0) alpha:(color.alpha() / 255.0)];
}

auto NSMakeImage(hiro::Image image, uint scaleWidth = 0, uint scaleHeight = 0) -> NSImage* {
  if(!image.state.data) return nil;

  //convert ARGB8888 to ABGR8888
  auto p = image.data();
  for(auto n : range(image.width() * image.height())) {
    uint32 color = *p;
    color = (color & 0xff00ff00) | ((color & 0xff0000) >> 16) | ((color & 0x0000ff) << 16);
    *p++ = color;
  }

  //create NSImage from memory
  NSImage* cocoaImage = [[[NSImage alloc] initWithSize:NSMakeSize(image.width(), image.height())] autorelease];
  NSBitmapImageRep* bitmap = [[[NSBitmapImageRep alloc]
    initWithBitmapDataPlanes:nil
    pixelsWide:image.width() pixelsHigh:image.height()
    bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
    isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace
    bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
    bytesPerRow:4*image.width() bitsPerPixel:32
  ] autorelease];
  memory::copy([bitmap bitmapData], image.data(), 4 * image.width() * image.height());
  [cocoaImage addRepresentation:bitmap];
  if(!scaleWidth || !scaleHeight) return cocoaImage;

  //scale image
  [cocoaImage setScalesWhenResized:YES];
  NSImage* scaleImage = [[[NSImage alloc] initWithSize:NSMakeSize(scaleWidth, scaleHeight)] autorelease];
  [scaleImage lockFocus];
  [cocoaImage setSize:NSMakeSize(scaleWidth, scaleHeight)];
  [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
  [cocoaImage drawAtPoint:NSZeroPoint fromRect:CGRectMake(0, 0, scaleWidth, scaleHeight) operation:NSCompositeCopy fraction:1.0];
  [scaleImage unlockFocus];
  return scaleImage;
}

auto DropPathsOperation(id<NSDraggingInfo> sender) -> NSDragOperation {
  NSPasteboard* pboard = [sender draggingPasteboard];
  if([[pboard types] containsObject:NSFilenamesPboardType]) {
    if([sender draggingSourceOperationMask] & NSDragOperationGeneric) {
      return NSDragOperationGeneric;
    }
  }
  return NSDragOperationNone;
}

auto DropPaths(id<NSDraggingInfo> sender) -> lstring {
  lstring paths;
  NSPasteboard* pboard = [sender draggingPasteboard];
  if([[pboard types] containsObject:NSFilenamesPboardType]) {
    NSArray* files = [pboard propertyListForType:NSFilenamesPboardType];
    for(uint n = 0; n < [files count]; n++) {
      string path = [[files objectAtIndex:n] UTF8String];
      if(directory::exists(path) && !path.endsWith("/")) path.append("/");
      paths.append(path);
    }
  }
  return paths;
}
