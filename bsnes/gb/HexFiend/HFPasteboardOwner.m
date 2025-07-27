//
//  HFPasteboardOwner.m
//  HexFiend_2
//
//  Copyright 2008 ridiculous_fish. All rights reserved.
//

#import <HexFiend/HFPasteboardOwner.h>
#import <HexFiend/HFController.h>
#import <HexFiend/HFByteArray.h>
#import <objc/message.h>

NSString *const HFPrivateByteArrayPboardType = @"HFPrivateByteArrayPboardType";

@implementation HFPasteboardOwner

+ (void)initialize {
    if (self == [HFPasteboardOwner class]) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(prepareCommonPasteboardsForChangeInFileNotification:) name:HFPrepareForChangeInFileNotification object:nil];
    }
}

- (instancetype)initWithPasteboard:(NSPasteboard *)pboard forByteArray:(HFByteArray *)array withTypes:(NSArray *)types {
    REQUIRE_NOT_NULL(pboard);
    REQUIRE_NOT_NULL(array);
    REQUIRE_NOT_NULL(types);
    self = [super init];
    byteArray = [array retain];
    pasteboard = pboard;
    [pasteboard declareTypes:types owner:self];
    
    // get notified when we're about to write a file, so that if they're overwriting a file backing part of our byte array, we can properly clear or preserve our pasteboard
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(changeInFileNotification:) name:HFPrepareForChangeInFileNotification object:nil];
    
    return self;
}
+ (id)ownPasteboard:(NSPasteboard *)pboard forByteArray:(HFByteArray *)array withTypes:(NSArray *)types {
    return [[[self alloc] initWithPasteboard:pboard forByteArray:array withTypes:types] autorelease];
}

- (void)tearDownPasteboardReferenceIfExists {
    if (pasteboard) {
        pasteboard = nil;
        [[NSNotificationCenter defaultCenter] removeObserver:self name:HFPrepareForChangeInFileNotification object:nil];
    }
    if (retainedSelfOnBehalfOfPboard) {
        [self release];
        retainedSelfOnBehalfOfPboard = NO;
    }
}


+ (HFByteArray *)_unpackByteArrayFromDictionary:(NSDictionary *)byteArrayDictionary {
    HFByteArray *result = nil;
    if (byteArrayDictionary) {
        NSString *uuid = byteArrayDictionary[@"HFUUID"];
        if ([uuid isEqual:[self uuid]]) {
            result = (HFByteArray *)[byteArrayDictionary[@"HFByteArray"] unsignedLongValue];
        }
    }
    return result;
}

+ (HFByteArray *)unpackByteArrayFromPasteboard:(NSPasteboard *)pasteboard {
    REQUIRE_NOT_NULL(pasteboard);
    HFByteArray *result = [self _unpackByteArrayFromDictionary:[pasteboard propertyListForType:HFPrivateByteArrayPboardType]];
    return result;
}

/* Try to fix up commonly named pasteboards when a file is about to be saved */
+ (void)prepareCommonPasteboardsForChangeInFileNotification:(NSNotification *)notification {
    const BOOL *cancellationPointer = [[notification userInfo][HFChangeInFileShouldCancelKey] pointerValue];
    if (*cancellationPointer) return; //don't do anything if someone requested cancellation
    
    NSDictionary *userInfo = [notification userInfo];
    NSArray *changedRanges = userInfo[HFChangeInFileModifiedRangesKey];
    HFFileReference *fileReference = [notification object];
    NSMutableDictionary *hint = userInfo[HFChangeInFileHintKey];
    
    NSString * const names[] = {NSGeneralPboard, NSFindPboard, NSDragPboard};
    NSUInteger i;
    for (i=0; i < sizeof names / sizeof *names; i++) {
        NSPasteboard *pboard = [NSPasteboard pasteboardWithName:names[i]];
        HFByteArray *byteArray = [self unpackByteArrayFromPasteboard:pboard];
        if (byteArray && ! [byteArray clearDependenciesOnRanges:changedRanges inFile:fileReference hint:hint]) {
            /* This pasteboard no longer works */
            [pboard declareTypes:@[] owner:nil];
        }
    }
}

- (void)changeInFileNotification:(NSNotification *)notification {
    HFASSERT(pasteboard != nil);
    HFASSERT(byteArray != nil);
    NSDictionary *userInfo = [notification userInfo];
    const BOOL *cancellationPointer = [userInfo[HFChangeInFileShouldCancelKey] pointerValue];
    if (*cancellationPointer) return; //don't do anything if someone requested cancellation
    NSMutableDictionary *hint = userInfo[HFChangeInFileHintKey];
    
    NSArray *changedRanges = [notification userInfo][HFChangeInFileModifiedRangesKey];
    HFFileReference *fileReference = [notification object];
    if (! [byteArray clearDependenciesOnRanges:changedRanges inFile:fileReference hint:hint]) {
        /* We can't do it */
        [self tearDownPasteboardReferenceIfExists];
    }
}

- (void)dealloc {
    [self tearDownPasteboardReferenceIfExists];
    [byteArray release];
    [super dealloc];
}

- (void)writeDataInBackgroundToPasteboard:(NSPasteboard *)pboard ofLength:(unsigned long long)length forType:(NSString *)type trackingProgress:(id)tracker {
    USE(length);
    USE(pboard);
    USE(type);
    USE(tracker);
    UNIMPLEMENTED_VOID();
}

- (void)backgroundMoveDataToPasteboard:(NSString *)type {
    @autoreleasepool {
    [self writeDataInBackgroundToPasteboard:pasteboard ofLength:dataAmountToCopy forType:type trackingProgress:nil];
    [self performSelectorOnMainThread:@selector(backgroundMoveDataFinished:) withObject:nil waitUntilDone:NO];
    }
}

- (void)backgroundMoveDataFinished:unused {
    USE(unused);
    HFASSERT(backgroundCopyOperationFinished == NO);
    backgroundCopyOperationFinished = YES;
    if (! didStartModalSessionForBackgroundCopyOperation) {
        /* We haven't started the modal session, so make sure it never happens */
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(beginModalSessionForBackgroundCopyOperation:) object:nil];
        CFRunLoopWakeUp(CFRunLoopGetCurrent());
    }
    else {
        /* We have started the modal session, so end it. */
        [NSApp stopModalWithCode:0];
        //stopModal: won't trigger unless we post a do-nothing event
        NSEvent *event = [NSEvent otherEventWithType:NSApplicationDefined location:NSZeroPoint modifierFlags:0 timestamp:0 windowNumber:0 context:NULL subtype:0 data1:0 data2:0];
        [NSApp postEvent:event atStart:NO];
    }
}

- (void)beginModalSessionForBackgroundCopyOperation:(id)unused {
    USE(unused);
    HFASSERT(backgroundCopyOperationFinished == NO);
    HFASSERT(didStartModalSessionForBackgroundCopyOperation == NO);
    didStartModalSessionForBackgroundCopyOperation = YES;
}

- (BOOL)moveDataWithProgressReportingToPasteboard:(NSPasteboard *)pboard forType:(NSString *)type {
    // The -[NSRunLoop runMode:beforeDate:] call in the middle of this function can cause it to be
    // called reentrantly, which was previously causing leaks and use-after-free crashes. For
    // some reason this happens basically always when copying lots of data into VMware Fusion.
    // I'm not even sure what the ideal behavior would be here, but am fairly certain that this
    // is the best that can be done without rewriting a portion of the background copying code.
    // TODO: Figure out what the ideal behavior should be here.
    
    HFASSERT(pboard == pasteboard);
    [self retain]; //resolving the pasteboard may release us, which deallocates us, which deallocates our tracker...make sure we survive through this function
    /* Give the user a chance to request a smaller amount if it's really big */
    unsigned long long availableAmount = [byteArray length];
    unsigned long long amountToCopy = [self amountToCopyForDataLength:availableAmount stringLength:[self stringLengthForDataLength:availableAmount]];
    if (amountToCopy > 0) {

        backgroundCopyOperationFinished = NO;
        didStartModalSessionForBackgroundCopyOperation = NO;
        dataAmountToCopy = amountToCopy;
        [NSThread detachNewThreadSelector:@selector(backgroundMoveDataToPasteboard:) toTarget:self withObject:type];
        [self performSelector:@selector(beginModalSessionForBackgroundCopyOperation:) withObject:nil afterDelay:1.0 inModes:@[NSModalPanelRunLoopMode]];
        while (! backgroundCopyOperationFinished) {
            [[NSRunLoop currentRunLoop] runMode:NSModalPanelRunLoopMode beforeDate:[NSDate distantFuture]];
        }
    }
    [self release];
    return YES;
}

- (void)pasteboardChangedOwner:(NSPasteboard *)pboard {
    HFASSERT(pasteboard == pboard);
    [self tearDownPasteboardReferenceIfExists];
}

- (HFByteArray *)byteArray {
    return byteArray;
}

- (void)pasteboard:(NSPasteboard *)pboard provideDataForType:(NSString *)type {
    if (! pasteboard) {
        /* Don't do anything, because we've torn down our pasteboard */
        return;
    }
    if ([type isEqualToString:HFPrivateByteArrayPboardType]) {
        if (! retainedSelfOnBehalfOfPboard) {
            retainedSelfOnBehalfOfPboard = YES;
            CFRetain(self);
        }
        NSDictionary *dict = @{@"HFByteArray": @((unsigned long)byteArray),
                              @"HFUUID": [[self class] uuid]};
        [pboard setPropertyList:dict forType:type];
    }
    else {
        if (! [self moveDataWithProgressReportingToPasteboard:pboard forType:type]) {
            [pboard setData:[NSData data] forType:type];
        }
    }
}

- (void)setBytesPerLine:(NSUInteger)val { bytesPerLine = val; }
- (NSUInteger)bytesPerLine { return bytesPerLine; }

+ (NSString *)uuid {
    static NSString *uuid;
    if (! uuid) {
        CFUUIDRef uuidRef = CFUUIDCreate(NULL);
        uuid = (NSString *)CFUUIDCreateString(NULL, uuidRef);
        CFRelease(uuidRef);
    }
    return uuid;
}

- (unsigned long long)stringLengthForDataLength:(unsigned long long)dataLength { USE(dataLength); UNIMPLEMENTED(); }

- (unsigned long long)amountToCopyForDataLength:(unsigned long long)numBytes stringLength:(unsigned long long)stringLength {
    unsigned long long dataLengthResult, stringLengthResult;
    NSInteger alertReturn = NSIntegerMax;
    const unsigned long long copyOption1 = MAXIMUM_PASTEBOARD_SIZE_TO_EXPORT;
    const unsigned long long copyOption2 = MINIMUM_PASTEBOARD_SIZE_TO_WARN_ABOUT;
    NSString *option1String = HFDescribeByteCount(copyOption1);
    NSString *option2String = HFDescribeByteCount(copyOption2);
    NSString* dataSizeDescription = HFDescribeByteCount(stringLength);
    if (stringLength >= MAXIMUM_PASTEBOARD_SIZE_TO_EXPORT) {
        NSString *option1 = [@"Copy " stringByAppendingString:option1String];
        NSString *option2 = [@"Copy " stringByAppendingString:option2String];
        alertReturn = NSRunAlertPanel(@"Large Clipboard", @"The copied data would occupy %@ if written to the clipboard.  This is larger than the system clipboard supports.  Do you want to copy only part of the data?", @"Cancel",  option1, option2, dataSizeDescription);
        switch (alertReturn) {
            case NSAlertDefaultReturn:
            default:
                stringLengthResult = 0;
                break;
            case NSAlertAlternateReturn:
                stringLengthResult = copyOption1;
                break;
            case NSAlertOtherReturn:
                stringLengthResult = copyOption2;
                break;
        }
        
    }
    else if (stringLength >= MINIMUM_PASTEBOARD_SIZE_TO_WARN_ABOUT) {
        NSString *option1 = [@"Copy " stringByAppendingString:HFDescribeByteCount(stringLength)];
        NSString *option2 = [@"Copy " stringByAppendingString:HFDescribeByteCount(copyOption2)];
        alertReturn = NSRunAlertPanel(@"Large Clipboard", @"The copied data would occupy %@ if written to the clipboard.  Performing this copy may take a long time.  Do you want to copy only part of the data?", @"Cancel",  option1, option2, dataSizeDescription);
        switch (alertReturn) {
            case NSAlertDefaultReturn:
            default:
                stringLengthResult = 0;
                break;
            case NSAlertAlternateReturn:
                stringLengthResult = stringLength;
                break;
            case NSAlertOtherReturn:
                stringLengthResult = copyOption2;
                break;
        }
    }
    else {
        /* Small enough to copy it all */
        stringLengthResult = stringLength;
    }
    
    /* Convert from string length to data length */
    if (stringLengthResult == stringLength) {
        dataLengthResult = numBytes;
    }
    else {
        unsigned long long divisor = stringLength / numBytes;
        dataLengthResult = stringLengthResult / divisor;
    }
    
    return dataLengthResult;
}

@end
