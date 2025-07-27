//
//  HFLineCountingView.m
//  HexFiend_2
//
//  Copyright 2007 ridiculous_fish. All rights reserved.
//

#import <HexFiend/HFLineCountingView.h>
#import <HexFiend/HFLineCountingRepresenter.h>
#import <HexFiend/HFFunctions.h>

#define TIME_LINE_NUMBERS 0

#define HEX_LINE_NUMBERS_HAVE_0X_PREFIX 0

#define INVALID_LINE_COUNT NSUIntegerMax

#if TIME_LINE_NUMBERS
@interface HFTimingTextView : NSTextView
@end
@implementation HFTimingTextView
- (void)drawRect:(NSRect)rect {
    CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    [super drawRect:rect];
    CFAbsoluteTime endTime = CFAbsoluteTimeGetCurrent();
    NSLog(@"TextView line number time: %f", endTime - startTime);
}
@end
#endif

@implementation HFLineCountingView

- (void)_sharedInitLineCountingView {
    layoutManager = [[NSLayoutManager alloc] init];
    textStorage = [[NSTextStorage alloc] init];
    [textStorage addLayoutManager:layoutManager];
    textContainer = [[NSTextContainer alloc] init];
    [textContainer setLineFragmentPadding:(CGFloat)5];
    [textContainer setContainerSize:NSMakeSize(self.bounds.size.width, [textContainer containerSize].height)];
    [layoutManager addTextContainer:textContainer];
}

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self _sharedInitLineCountingView];
    }
    return self;
}

- (void)dealloc {
    HFUnregisterViewForWindowAppearanceChanges(self, registeredForAppNotifications);
    [_font release];
    [layoutManager release];
    [textContainer release];
    [textStorage release];
    [textAttributes release];
    [super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)coder {
    HFASSERT([coder allowsKeyedCoding]);
    [super encodeWithCoder:coder];
    [coder encodeObject:_font forKey:@"HFFont"];
    [coder encodeDouble:_lineHeight forKey:@"HFLineHeight"];
    [coder encodeObject:_representer forKey:@"HFRepresenter"];
    [coder encodeInt64:_bytesPerLine forKey:@"HFBytesPerLine"];
    [coder encodeInt64:_lineNumberFormat forKey:@"HFLineNumberFormat"];
    [coder encodeBool:useStringDrawingPath forKey:@"HFUseStringDrawingPath"];
}

- (instancetype)initWithCoder:(NSCoder *)coder {
    HFASSERT([coder allowsKeyedCoding]);
    self = [super initWithCoder:coder];
    [self _sharedInitLineCountingView];
    _font = [[coder decodeObjectForKey:@"HFFont"] retain];
    _lineHeight = (CGFloat)[coder decodeDoubleForKey:@"HFLineHeight"];
    _representer = [coder decodeObjectForKey:@"HFRepresenter"];
    _bytesPerLine = (NSUInteger)[coder decodeInt64ForKey:@"HFBytesPerLine"];
    _lineNumberFormat = (NSUInteger)[coder decodeInt64ForKey:@"HFLineNumberFormat"];
    useStringDrawingPath = [coder decodeBoolForKey:@"HFUseStringDrawingPath"];
    return self;
}

- (BOOL)isFlipped { return YES; }

- (void)getLineNumberFormatString:(char *)outString length:(NSUInteger)length {
    HFLineNumberFormat format = self.lineNumberFormat;
    if (format == HFLineNumberFormatDecimal) {
        strlcpy(outString, "%llu", length);
    }
    else if (format == HFLineNumberFormatHexadecimal) {
#if HEX_LINE_NUMBERS_HAVE_0X_PREFIX
        // we want a format string like 0x%08llX
        snprintf(outString, length, "0x%%0%lullX", (unsigned long)self.representer.digitCount - 2);
#else
        // we want a format string like %08llX
        snprintf(outString, length, "%%0%lullX", (unsigned long)self.representer.digitCount);
#endif
    }
    else {
        strlcpy(outString, "", length);
    }
}

- (void)windowDidChangeKeyStatus:(NSNotification *)note {
    USE(note);
    [self setNeedsDisplay:YES];
}

- (void)viewDidMoveToWindow {
    HFRegisterViewForWindowAppearanceChanges(self, @selector(windowDidChangeKeyStatus:), !registeredForAppNotifications);
    registeredForAppNotifications = YES;
    [super viewDidMoveToWindow];
}

- (void)viewWillMoveToWindow:(NSWindow *)newWindow {
    HFUnregisterViewForWindowAppearanceChanges(self, NO);
    [super viewWillMoveToWindow:newWindow];
}

- (void)drawDividerWithClip:(NSRect)clipRect {
    USE(clipRect);
    

#if 1    
    NSInteger edges = _representer.borderedEdges;
    NSRect bounds = self.bounds;
    
    
    // -1 means to draw no edges
    if (edges == -1) {
        edges = 0;
    }
    
    [_representer.borderColor set];
    
    if ((edges & (1 << NSMinXEdge)) > 0) {
        NSRect lineRect = bounds;
        lineRect.size.width = 1;
        lineRect.origin.x = 0;
        if (NSIntersectsRect(lineRect, clipRect)) {
            NSRectFill(lineRect);
        }
    }
    
    if ((edges & (1 << NSMaxXEdge)) > 0) {
        NSRect lineRect = bounds;
        lineRect.size.width = 1;
        lineRect.origin.x = NSMaxX(bounds) - lineRect.size.width;
        if (NSIntersectsRect(lineRect, clipRect)) {
            NSRectFill(lineRect);
        }
    }
    
    if ((edges & (1 << NSMinYEdge)) > 0) {
        NSRect lineRect = bounds;
        lineRect.size.height = 1;
        lineRect.origin.y = 0;
        if (NSIntersectsRect(lineRect, clipRect)) {
            NSRectFill(lineRect);
        }
    }
    
    if ((edges & (1 << NSMaxYEdge)) > 0) {
        NSRect lineRect = bounds;
        lineRect.size.height = 1;
        lineRect.origin.y = NSMaxY(bounds) - lineRect.size.height;
        if (NSIntersectsRect(lineRect, clipRect)) {
            NSRectFill(lineRect);
        }
    }
    
    
    // Backwards compatibility to always draw a border on the edge with the interior shadow
    
    NSRect lineRect = bounds;
    lineRect.size.width = 1;
    NSInteger shadowEdge = _representer.interiorShadowEdge;
    if (shadowEdge == NSMaxXEdge) {
        lineRect.origin.x = NSMaxX(bounds) - lineRect.size.width;
    } else if (shadowEdge == NSMinXEdge) {
        lineRect.origin.x = NSMinX(bounds);
    } else {
        lineRect = NSZeroRect;
    }
    
    if (NSIntersectsRect(lineRect, clipRect)) {
        NSRectFill(lineRect);
    }
    
#else
    
    
    if (NSIntersectsRect(lineRect, clipRect)) {
        // this looks better when we have no shadow
        [[NSColor lightGrayColor] set];
        NSRect bounds = self.bounds;
        NSRect lineRect = bounds;
        lineRect.origin.x += lineRect.size.width - 2;
        lineRect.size.width = 1;
        NSRectFill(NSIntersectionRect(lineRect, clipRect));
        [[NSColor whiteColor] set];
        lineRect.origin.x += 1;
        NSRectFill(NSIntersectionRect(lineRect, clipRect));	
    }
#endif
}

static inline int common_prefix_length(const char *a, const char *b) {
    int i;
    for (i=0; ; i++) {
        char ac = a[i];
        char bc = b[i];
        if (ac != bc || ac == 0 || bc == 0) break;
    }
    return i;
}

/* Drawing with NSLayoutManager is necessary because the 10_2 typesetting behavior used by the old string drawing does the wrong thing for fonts like Bitstream Vera Sans Mono.  Also it's an optimization for drawing the shadow. */
- (void)drawLineNumbersWithClipLayoutManagerPerLine:(NSRect)clipRect {
#if TIME_LINE_NUMBERS
    CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
#endif
    NSUInteger previousTextStorageCharacterCount = [textStorage length];
    
    CGFloat verticalOffset = ld2f(_lineRangeToDraw.location - floorl(_lineRangeToDraw.location));
    NSRect textRect = self.bounds;
    textRect.size.height = _lineHeight;
    textRect.origin.y -= verticalOffset * _lineHeight;
    unsigned long long lineIndex = HFFPToUL(floorl(_lineRangeToDraw.location));
    unsigned long long lineValue = lineIndex * _bytesPerLine;
    NSUInteger linesRemaining = ll2l(HFFPToUL(ceill(_lineRangeToDraw.length + _lineRangeToDraw.location) - floorl(_lineRangeToDraw.location)));
    char previousBuff[256];
    int previousStringLength = (int)previousTextStorageCharacterCount;
    BOOL conversionResult = [[textStorage string] getCString:previousBuff maxLength:sizeof previousBuff encoding:NSASCIIStringEncoding];
    HFASSERT(conversionResult);
    while (linesRemaining--) {
        char formatString[64];
        [self getLineNumberFormatString:formatString length:sizeof formatString];
        
        if (NSIntersectsRect(textRect, clipRect)) {
            NSString *replacementCharacters = nil;
            NSRange replacementRange;
            char buff[256];
            int newStringLength = snprintf(buff, sizeof buff, formatString, lineValue);
            HFASSERT(newStringLength > 0);
            int prefixLength = common_prefix_length(previousBuff, buff);
            HFASSERT(prefixLength <= newStringLength);
            HFASSERT(prefixLength <= previousStringLength);
            replacementRange = NSMakeRange(prefixLength, previousStringLength - prefixLength);
            replacementCharacters = [[NSString alloc] initWithBytesNoCopy:buff + prefixLength length:newStringLength - prefixLength encoding:NSASCIIStringEncoding freeWhenDone:NO];
            NSUInteger glyphCount;
            [textStorage replaceCharactersInRange:replacementRange withString:replacementCharacters];
            if (previousTextStorageCharacterCount == 0) {
                NSDictionary *atts = [[NSDictionary alloc] initWithObjectsAndKeys:_font, NSFontAttributeName, [NSColor controlTextColor], NSForegroundColorAttributeName, nil];
                [textStorage setAttributes:atts range:NSMakeRange(0, newStringLength)];
                [atts release];
            }
            glyphCount = [layoutManager numberOfGlyphs];
            if (glyphCount > 0) {
                CGFloat maxX = NSMaxX([layoutManager lineFragmentUsedRectForGlyphAtIndex:glyphCount - 1 effectiveRange:NULL]);
                [layoutManager drawGlyphsForGlyphRange:NSMakeRange(0, glyphCount) atPoint:NSMakePoint(textRect.origin.x + textRect.size.width - maxX, textRect.origin.y)];
            }
            previousTextStorageCharacterCount = newStringLength;
            [replacementCharacters release];
            memcpy(previousBuff, buff, newStringLength + 1);
            previousStringLength = newStringLength;
        }
        textRect.origin.y += _lineHeight;
        lineIndex++;
        lineValue = HFSum(lineValue, _bytesPerLine);
    }
#if TIME_LINE_NUMBERS
    CFAbsoluteTime endTime = CFAbsoluteTimeGetCurrent();
    NSLog(@"Line number time: %f", endTime - startTime);
#endif
}

- (void)drawLineNumbersWithClipStringDrawing:(NSRect)clipRect {
    CGFloat verticalOffset = ld2f(_lineRangeToDraw.location - floorl(_lineRangeToDraw.location));
    NSRect textRect = self.bounds;
    textRect.size.height = _lineHeight;
    textRect.size.width -= 5;
    textRect.origin.y -= verticalOffset * _lineHeight + 1;
    unsigned long long lineIndex = HFFPToUL(floorl(_lineRangeToDraw.location));
    unsigned long long lineValue = lineIndex * _bytesPerLine;
    NSUInteger linesRemaining = ll2l(HFFPToUL(ceill(_lineRangeToDraw.length + _lineRangeToDraw.location) - floorl(_lineRangeToDraw.location)));
    if (! textAttributes) {
        NSMutableParagraphStyle *mutableStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        [mutableStyle setAlignment:NSRightTextAlignment];
        NSParagraphStyle *paragraphStyle = [mutableStyle copy];
        [mutableStyle release];
        textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:_font, NSFontAttributeName, [NSColor controlTextColor], NSForegroundColorAttributeName, paragraphStyle, NSParagraphStyleAttributeName, nil];
        [paragraphStyle release];
    }
    
    char formatString[64];
    [self getLineNumberFormatString:formatString length:sizeof formatString];
    
    while (linesRemaining--) {
        if (NSIntersectsRect(textRect, clipRect)) {
            char buff[256];
            int newStringLength = snprintf(buff, sizeof buff, formatString, lineValue);
            HFASSERT(newStringLength > 0);
            NSString *string = [[NSString alloc] initWithBytesNoCopy:buff length:newStringLength encoding:NSASCIIStringEncoding freeWhenDone:NO];
            [string drawInRect:textRect withAttributes:textAttributes];
            [string release];
        }
        textRect.origin.y += _lineHeight;
        lineIndex++;
        if (linesRemaining > 0) lineValue = HFSum(lineValue, _bytesPerLine); //we could do this unconditionally, but then we risk overflow
    }
}

- (NSUInteger)characterCountForLineRange:(HFRange)range {
    HFASSERT(range.length <= NSUIntegerMax);
    NSUInteger characterCount;
    
    NSUInteger lineCount = ll2l(range.length);
    const NSUInteger stride = _bytesPerLine;
    HFLineCountingRepresenter *rep = self.representer;
    HFLineNumberFormat format = self.lineNumberFormat;
    if (format == HFLineNumberFormatDecimal) {
        unsigned long long lineValue = HFProductULL(range.location, _bytesPerLine);
        characterCount = lineCount /* newlines */;
        while (lineCount--) {
            characterCount += HFCountDigitsBase10(lineValue);
            lineValue += stride;
        }
    }
    else if (format == HFLineNumberFormatHexadecimal) {
        characterCount = ([rep digitCount] + 1) * lineCount; // +1 for newlines
    }
    else {
        characterCount = -1;
    }
    return characterCount;
}

- (NSString *)newLineStringForRange:(HFRange)range {
    HFASSERT(range.length <= NSUIntegerMax);
    if(range.length == 0)
        return [[NSString alloc] init]; // Placate the analyzer.
    
    NSUInteger lineCount = ll2l(range.length);
    const NSUInteger stride = _bytesPerLine;
    unsigned long long lineValue = HFProductULL(range.location, _bytesPerLine);
    NSUInteger characterCount = [self characterCountForLineRange:range];
    char *buffer = check_malloc(characterCount);
    NSUInteger bufferIndex = 0;
    
    char formatString[64];
    [self getLineNumberFormatString:formatString length:sizeof formatString];
    
    while (lineCount--) {
        int charCount = sprintf(buffer + bufferIndex, formatString, lineValue + self.representer.valueOffset);
        HFASSERT(charCount > 0);
        bufferIndex += charCount;
        buffer[bufferIndex++] = '\n';   
        lineValue += stride;
    }
    HFASSERT(bufferIndex == characterCount);
    
    NSString *string = [[NSString alloc] initWithBytesNoCopy:(void *)buffer length:bufferIndex encoding:NSASCIIStringEncoding freeWhenDone:YES];
    return string;
}

- (void)updateLayoutManagerWithLineIndex:(unsigned long long)startingLineIndex lineCount:(NSUInteger)linesRemaining {
    const BOOL debug = NO;
    [textStorage beginEditing];
    
    if (storedLineCount == INVALID_LINE_COUNT) {
        /* This usually indicates that our bytes per line or line number format changed, and we need to just recalculate everything */
        NSString *string = [self newLineStringForRange:HFRangeMake(startingLineIndex, linesRemaining)];
        [textStorage replaceCharactersInRange:NSMakeRange(0, [textStorage length]) withString:string];
        [string release];
        
    }
    else {
        HFRange leftRangeToReplace, rightRangeToReplace;
        HFRange leftRangeToStore, rightRangeToStore;
        
        HFRange oldRange = HFRangeMake(storedLineIndex, storedLineCount);
        HFRange newRange = HFRangeMake(startingLineIndex, linesRemaining);
        HFRange rangeToPreserve = HFIntersectionRange(oldRange, newRange);
        
        if (rangeToPreserve.length == 0) {
            leftRangeToReplace = oldRange;
            leftRangeToStore = newRange;
            rightRangeToReplace = HFZeroRange;
            rightRangeToStore = HFZeroRange;
        }
        else {
            if (debug) NSLog(@"Preserving %llu", rangeToPreserve.length);
            HFASSERT(HFRangeIsSubrangeOfRange(rangeToPreserve, newRange));
            HFASSERT(HFRangeIsSubrangeOfRange(rangeToPreserve, oldRange));
            const unsigned long long maxPreserve = HFMaxRange(rangeToPreserve);
            leftRangeToReplace = HFRangeMake(oldRange.location, rangeToPreserve.location - oldRange.location);
            leftRangeToStore = HFRangeMake(newRange.location, rangeToPreserve.location - newRange.location);
            rightRangeToReplace = HFRangeMake(maxPreserve, HFMaxRange(oldRange) - maxPreserve);
            rightRangeToStore = HFRangeMake(maxPreserve, HFMaxRange(newRange) - maxPreserve);
        }
        
        if (debug) NSLog(@"Changing %@ -> %@", HFRangeToString(oldRange), HFRangeToString(newRange));
        if (debug) NSLog(@"LEFT: %@ -> %@", HFRangeToString(leftRangeToReplace), HFRangeToString(leftRangeToStore));
        if (debug) NSLog(@"RIGHT: %@ -> %@", HFRangeToString(rightRangeToReplace), HFRangeToString(rightRangeToStore));
        
        HFASSERT(leftRangeToReplace.length == 0 || HFRangeIsSubrangeOfRange(leftRangeToReplace, oldRange));
        HFASSERT(rightRangeToReplace.length == 0 || HFRangeIsSubrangeOfRange(rightRangeToReplace, oldRange));
        
        if (leftRangeToReplace.length > 0 || leftRangeToStore.length > 0) {
            NSUInteger charactersToDelete = [self characterCountForLineRange:leftRangeToReplace];
            NSRange rangeToDelete = NSMakeRange(0, charactersToDelete);
            if (leftRangeToStore.length == 0) {
                [textStorage deleteCharactersInRange:rangeToDelete];
                if (debug) NSLog(@"Left deleting text range %@", NSStringFromRange(rangeToDelete));
            }
            else {
                NSString *leftRangeString = [self newLineStringForRange:leftRangeToStore];
                [textStorage replaceCharactersInRange:rangeToDelete withString:leftRangeString];
                if (debug) NSLog(@"Replacing text range %@ with %@", NSStringFromRange(rangeToDelete), leftRangeString);
                [leftRangeString release];
            }
        }
        
        if (rightRangeToReplace.length > 0 || rightRangeToStore.length > 0) {
            NSUInteger charactersToDelete = [self characterCountForLineRange:rightRangeToReplace];
            NSUInteger stringLength = [textStorage length];
            HFASSERT(charactersToDelete <= stringLength);
            NSRange rangeToDelete = NSMakeRange(stringLength - charactersToDelete, charactersToDelete);
            if (rightRangeToStore.length == 0) {
                [textStorage deleteCharactersInRange:rangeToDelete];
                if (debug) NSLog(@"Right deleting text range %@", NSStringFromRange(rangeToDelete));
            }
            else {
                NSString *rightRangeString = [self newLineStringForRange:rightRangeToStore];
                [textStorage replaceCharactersInRange:rangeToDelete withString:rightRangeString];
                if (debug) NSLog(@"Replacing text range %@ with %@ (for range %@)", NSStringFromRange(rangeToDelete), rightRangeString, HFRangeToString(rightRangeToStore));
                [rightRangeString release];
            }
        }
    }
    
    if (!textAttributes) {
        NSMutableParagraphStyle *mutableStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        [mutableStyle setAlignment:NSRightTextAlignment];
        NSParagraphStyle *paragraphStyle = [mutableStyle copy];
        [mutableStyle release];
        textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:_font, NSFontAttributeName, [NSColor controlTextColor], NSForegroundColorAttributeName, paragraphStyle, NSParagraphStyleAttributeName, nil];
        [paragraphStyle release];
        [textStorage setAttributes:textAttributes range:NSMakeRange(0, [textStorage length])];
    }
    
    [textStorage endEditing];
    
#if ! NDEBUG
    NSString *comparisonString = [self newLineStringForRange:HFRangeMake(startingLineIndex, linesRemaining)];
    if (! [comparisonString isEqualToString:[textStorage string]]) {
        NSLog(@"Not equal!");
        NSLog(@"Expected:\n%@", comparisonString);
        NSLog(@"Actual:\n%@", [textStorage string]);
    }
    HFASSERT([comparisonString isEqualToString:[textStorage string]]);
    [comparisonString release];
#endif
    
    storedLineIndex = startingLineIndex;
    storedLineCount = linesRemaining;
}

- (void)drawLineNumbersWithClipSingleStringDrawing:(NSRect)clipRect {
    USE(clipRect);
    unsigned long long lineIndex = HFFPToUL(floorl(_lineRangeToDraw.location));
    NSUInteger linesRemaining = ll2l(HFFPToUL(ceill(_lineRangeToDraw.length + _lineRangeToDraw.location) - floorl(_lineRangeToDraw.location)));
    
    CGFloat linesToVerticallyOffset = ld2f(_lineRangeToDraw.location - floorl(_lineRangeToDraw.location));
    CGFloat verticalOffset = linesToVerticallyOffset * _lineHeight + 1.5;
    NSRect textRect = self.bounds;
    textRect.size.width -= 5;
    textRect.origin.y -= verticalOffset;
    textRect.size.height += verticalOffset + _lineHeight;
    
    NSMutableParagraphStyle *mutableStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    [mutableStyle setAlignment:NSRightTextAlignment];
    [mutableStyle setMinimumLineHeight:_lineHeight];
    [mutableStyle setMaximumLineHeight:_lineHeight];
    NSParagraphStyle *paragraphStyle = [mutableStyle copy];
    [mutableStyle release];
    NSColor *color = [[NSColor textColor] colorUsingColorSpace:[NSColorSpace deviceRGBColorSpace]];
    color = [NSColor colorWithRed:color.redComponent green:color.greenComponent blue:color.blueComponent alpha:0.75];
    NSDictionary *_textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:[NSFont fontWithName:_font.fontName size:9], NSFontAttributeName, color, NSForegroundColorAttributeName, paragraphStyle, NSParagraphStyleAttributeName, nil];
    [paragraphStyle release];
    

    NSString *string = [self newLineStringForRange:HFRangeMake(lineIndex, linesRemaining)];
    [string drawInRect:textRect withAttributes:_textAttributes];
    [string release];
    [_textAttributes release];
}

- (void)drawLineNumbersWithClipSingleStringCellDrawing:(NSRect)clipRect {
    USE(clipRect);
    const CGFloat cellTextContainerPadding = 2.f;
    unsigned long long lineIndex = HFFPToUL(floorl(_lineRangeToDraw.location));
    NSUInteger linesRemaining = ll2l(HFFPToUL(ceill(_lineRangeToDraw.length + _lineRangeToDraw.location) - floorl(_lineRangeToDraw.location)));
    
    CGFloat linesToVerticallyOffset = ld2f(_lineRangeToDraw.location - floorl(_lineRangeToDraw.location));
    CGFloat verticalOffset = linesToVerticallyOffset * _lineHeight + 1;
    NSRect textRect = self.bounds;
    textRect.size.width -= 5;
    textRect.origin.y -= verticalOffset;
    textRect.origin.x += cellTextContainerPadding;
    textRect.size.height += verticalOffset;
    
    if (! textAttributes) {
        NSMutableParagraphStyle *mutableStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        [mutableStyle setAlignment:NSRightTextAlignment];
        [mutableStyle setMinimumLineHeight:_lineHeight];
        [mutableStyle setMaximumLineHeight:_lineHeight];
        NSParagraphStyle *paragraphStyle = [mutableStyle copy];
        [mutableStyle release];
        textAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:_font, NSFontAttributeName, [NSColor controlTextColor], NSForegroundColorAttributeName, paragraphStyle, NSParagraphStyleAttributeName, nil];
        [paragraphStyle release];
    }
    
    NSString *string = [self newLineStringForRange:HFRangeMake(lineIndex, linesRemaining)];
    NSAttributedString *attributedString = [[NSAttributedString alloc] initWithString:string attributes:textAttributes];
    [string release];
    NSCell *cell = [[NSCell alloc] initTextCell:@""];
    [cell setAttributedStringValue:attributedString];
    [cell drawWithFrame:textRect inView:self];
    [[NSColor purpleColor] set];
    NSFrameRect(textRect);
    [cell release];
    [attributedString release];
}

- (void)drawLineNumbersWithClipFullLayoutManager:(NSRect)clipRect {
    USE(clipRect);
    unsigned long long lineIndex = HFFPToUL(floorl(_lineRangeToDraw.location));
    NSUInteger linesRemaining = ll2l(HFFPToUL(ceill(_lineRangeToDraw.length + _lineRangeToDraw.location) - floorl(_lineRangeToDraw.location)));
    if (lineIndex != storedLineIndex || linesRemaining != storedLineCount) {
        [self updateLayoutManagerWithLineIndex:lineIndex lineCount:linesRemaining];
    }
    
    CGFloat verticalOffset = ld2f(_lineRangeToDraw.location - floorl(_lineRangeToDraw.location));
    
    NSPoint textPoint = self.bounds.origin;
    textPoint.y -= verticalOffset * _lineHeight;
    [layoutManager drawGlyphsForGlyphRange:NSMakeRange(0, [layoutManager numberOfGlyphs]) atPoint:textPoint];
}

- (void)drawLineNumbersWithClip:(NSRect)clipRect {
#if TIME_LINE_NUMBERS
    CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
#endif
    NSInteger drawingMode = 3; // (useStringDrawingPath ? 1 : 3);
    switch (drawingMode) {
        // Drawing can't be done right if fonts are wider than expected, but all
        // of these have rather nasty behavior in that case. I've commented what
        // that behavior is; the comment is hypothetical 'could' if it shouldn't
        // actually be a problem in practice.
        // TODO: Make a drawing mode that is "Fonts could get clipped if too wide"
        //       because that seems like better behavior than any of these.
        case 0:
            // Most fonts are too wide and every character gets piled on right (unreadable).
            [self drawLineNumbersWithClipLayoutManagerPerLine:clipRect];
            break;
        case 1:
            // Last characters could get omitted (*not* clipped) if too wide.
            // Also, most fonts have bottoms clipped (very unsigntly).
            [self drawLineNumbersWithClipStringDrawing:clipRect];
            break;
        case 2:
            // Most fonts are too wide and wrap (breaks numbering).
            [self drawLineNumbersWithClipFullLayoutManager:clipRect];
            break;
        case 3:
            // Fonts could wrap if too wide (breaks numbering).
            // *Note that that this is the only mode that generally works.*
            [self drawLineNumbersWithClipSingleStringDrawing:clipRect];
            break;
        case 4:
            // Most fonts are too wide and wrap (breaks numbering).
            [self drawLineNumbersWithClipSingleStringCellDrawing:clipRect];
            break;
    }
#if TIME_LINE_NUMBERS
    CFAbsoluteTime endTime = CFAbsoluteTimeGetCurrent();
    NSLog(@"Line number time: %f", endTime - startTime);
#endif
}

- (void)drawRect:(NSRect)clipRect {
    [self drawDividerWithClip:clipRect];
    [self drawLineNumbersWithClip:clipRect];
}

- (void)setLineRangeToDraw:(HFFPRange)range {
    if (! HFFPRangeEqualsRange(range, _lineRangeToDraw)) {
        _lineRangeToDraw = range;
        [self setNeedsDisplay:YES];
    }
}

- (void)setBytesPerLine:(NSUInteger)val {
    if (_bytesPerLine != val) {
        _bytesPerLine = val;
        storedLineCount = INVALID_LINE_COUNT;
        [self setNeedsDisplay:YES];
    }
}

- (void)setLineNumberFormat:(HFLineNumberFormat)format {
    if (format != _lineNumberFormat) {
        _lineNumberFormat = format;
        storedLineCount = INVALID_LINE_COUNT;
        [self setNeedsDisplay:YES];
    }
}

- (BOOL)canUseStringDrawingPathForFont:(NSFont *)testFont {
    NSString *name = [testFont fontName];
    // No, Menlo does not work here.
    return [name isEqualToString:@"Monaco"] || [name isEqualToString:@"Courier"] || [name isEqualToString:@"Consolas"];
}

- (void)setFont:(NSFont *)val {
    if (val != _font) {
        [_font release];
        _font = [val copy];
        [textStorage deleteCharactersInRange:NSMakeRange(0, [textStorage length])]; //delete the characters so we know to set the font next time we render
        [textAttributes release];
        textAttributes = nil;
        storedLineCount = INVALID_LINE_COUNT;
        useStringDrawingPath = [self canUseStringDrawingPathForFont:_font];
        [self setNeedsDisplay:YES];
    }
}

- (void)setLineHeight:(CGFloat)height {
    if (_lineHeight != height) {
        _lineHeight = height;
        [self setNeedsDisplay:YES];
    }
}

- (void)setFrameSize:(NSSize)size {
    [super setFrameSize:size];
    [textContainer setContainerSize:NSMakeSize(self.bounds.size.width, [textContainer containerSize].height)];
}

- (void)mouseDown:(NSEvent *)event {
    USE(event);
    // [_representer cycleLineNumberFormat];
}

- (void)scrollWheel:(NSEvent *)scrollEvent {
    [_representer.controller scrollWithScrollEvent:scrollEvent];
}

+ (NSUInteger)digitsRequiredToDisplayLineNumber:(unsigned long long)lineNumber inFormat:(HFLineNumberFormat)format {
    switch (format) {
        case HFLineNumberFormatDecimal: return HFCountDigitsBase10(lineNumber);
#if HEX_LINE_NUMBERS_HAVE_0X_PREFIX
        case HFLineNumberFormatHexadecimal: return 2 + HFCountDigitsBase16(lineNumber);
#else
        case HFLineNumberFormatHexadecimal: return HFCountDigitsBase16(lineNumber);
#endif
        default: return 0;
    }
}

/* Compatibility with Sonoma */
+ (bool)clipsToBounds
{
    return true;
}

@end
