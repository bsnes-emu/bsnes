auto NSMakeColor(const hiro::Color& color) -> NSColor* {
  return [NSColor colorWithRed:(color.red() / 255.0) green:(color.green() / 255.0) blue:(color.blue() / 255.0) alpha:(color.alpha() / 255.0)];
}

auto NSMakeImage(image icon, uint scaleWidth = 0, uint scaleHeight = 0) -> NSImage* {
  if(!icon) return nil;

  if(scaleWidth && scaleHeight) icon.scale(scaleWidth, scaleHeight);
  icon.transform(0, 32, 255u << 24, 255u << 0, 255u << 8, 255u << 16);  //Cocoa stores images in ABGR format

  //create NSImage from memory
  NSImage* cocoaImage = [[[NSImage alloc] initWithSize:NSMakeSize(icon.width(), icon.height())] autorelease];
  NSBitmapImageRep* bitmap = [[[NSBitmapImageRep alloc]
    initWithBitmapDataPlanes:nil
    pixelsWide:icon.width() pixelsHigh:icon.height()
    bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
    isPlanar:NO colorSpaceName:NSCalibratedRGBColorSpace
    bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
    bytesPerRow:(4 * icon.width()) bitsPerPixel:32
  ] autorelease];
  memory::copy([bitmap bitmapData], icon.data(), 4 * icon.width() * icon.height());
  [cocoaImage addRepresentation:bitmap];
  return cocoaImage;
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

auto DropPaths(id<NSDraggingInfo> sender) -> string_vector {
  string_vector paths;
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
