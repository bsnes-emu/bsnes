namespace phoenix {

string pFont::serif(unsigned size, string style) {
  if(size == 0) size = 12;
  if(style == "") style = "Normal";
  return {"Georgia, ", size, ", ", style};
}

string pFont::sans(unsigned size, string style) {
  if(size == 0) size = 12;
  if(style == "") style = "Normal";
  return {"Lucida Grande, ", size, ", ", style};
}

string pFont::monospace(unsigned size, string style) {
  if(size == 0) size = 12;
  if(style == "") style = "Normal";
  return {"Menlo, ", size, ", ", style};
}

Size pFont::size(const string& font, const string& text) {
  @autoreleasepool {
    if(NSFont* nsFont = cocoaFont(font)) {
      return size(nsFont, text);
    }
  }
  return {0, 0};
}

NSFont* pFont::cocoaFont(const string& description) {
  lstring part = description.split<2>(",");
  for(auto& item : part) item.strip();

  NSString* family = @"Lucida Grande";
  NSFontTraitMask traits = 0;
  CGFloat size = 12;

  if(!part(0).empty()) family = [NSString stringWithUTF8String:part(0)];
  if(!part(1).empty()) size = fp(part(1));
  if(part(2).iposition("bold")) traits |= NSBoldFontMask;
  if(part(2).iposition("italic")) traits |= NSItalicFontMask;
  if(part(2).iposition("narrow")) traits |= NSNarrowFontMask;
  if(part(2).iposition("expanded")) traits |= NSExpandedFontMask;
  if(part(2).iposition("condensed")) traits |= NSCondensedFontMask;
  if(part(2).iposition("smallcaps")) traits |= NSSmallCapsFontMask;

  return [[NSFontManager sharedFontManager] fontWithFamily:family traits:traits weight:5 size:size];
}

Size pFont::size(NSFont* font, const string& text) {
  @autoreleasepool {
    NSString* cocoaText = [NSString stringWithUTF8String:text];
    NSDictionary* fontAttributes = [NSDictionary dictionaryWithObjectsAndKeys:font, NSFontAttributeName, nil];
    NSSize size = [cocoaText sizeWithAttributes:fontAttributes];
    return {size.width, size.height};
  }
}

}
