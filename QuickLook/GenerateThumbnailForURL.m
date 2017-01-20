#include <QuickLook/QuickLook.h>
#include <Cocoa/Cocoa.h>
#include "get_image_for_rom.h"

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
    @autoreleasepool {
        /* Load the template NSImages when generating the first thumbnail */
        static NSImage *template = nil;
        static NSImage *templateUniversal = nil;
        static NSImage *templateColor = nil;
        static NSBundle *bundle = nil;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            bundle = [NSBundle bundleWithIdentifier:@"com.github.liji32.sameboy.previewer"];
            template = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"CartridgeTemplate" ofType:@"png"]];
            templateUniversal = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"UniversalCartridgeTemplate" ofType:@"png"]];
            templateColor = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"ColorCartridgeTemplate" ofType:@"png"]];
        });
        
        uint32_t bitmap[160*144];
        uint8_t cgbFlag = 0;

        /* The cgb_boot_fast boot ROM skips the boot animation */
        if (get_image_for_rom([[(__bridge NSURL *)url path] UTF8String],
                              [[bundle pathForResource:@"cgb_boot_fast" ofType:@"bin"] UTF8String],
                              bitmap, &cgbFlag, (cancel_callback_t)QLThumbnailRequestIsCancelled, thumbnail)) {
            return noErr;
        }
        
        /* Convert the screenshot to a CGImageRef */
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bitmap, sizeof(bitmap), NULL);
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        CGImageRef iref = CGImageCreate(160,
                                        144,
                                        8,
                                        32,
                                        4 * 160,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider,
                                        NULL,
                                        YES,
                                        renderingIntent);
        
        
        /* Draw the icon */
        CGContextRef cgContext = QLThumbnailRequestCreateContext(thumbnail, [template size], false, (__bridge CFDictionaryRef)(@{@"IconFlavor" : @(0)}));
        if (cgContext) {
            NSGraphicsContext *context = [NSGraphicsContext graphicsContextWithGraphicsPort:(void *)cgContext flipped:NO];
            if (context) {
                [NSGraphicsContext setCurrentContext:context];
                
                /* Use the CGB flag to determine the cartrdige "look":
                   - DMG cartridges are grey
                   - CGB cartrdiges are transparent
                   - CGB cartridges that support DMG systems are black
                 */
                NSImage *effectiveTemplate = nil;
                switch (cgbFlag) {
                    case 0xC0:
                        effectiveTemplate = templateColor;
                        break;
                    case 0x80:
                        effectiveTemplate = templateUniversal;
                        break;
                    default:
                        effectiveTemplate = template;
                }
                
                /* Convert the screenshot to a magnified NSImage */
                NSImage *screenshot = [[NSImage alloc] initWithCGImage:iref size:NSMakeSize(640, 576)];
                /* Draw the screenshot */
                [screenshot drawInRect:NSMakeRect(192, 150, 640, 576)];
                /* Mask it with the template (The middle part of the template image is transparent) */
                [effectiveTemplate drawInRect:(NSRect){{0,0},template.size}];
            }
            QLThumbnailRequestFlushContext(thumbnail, cgContext);
            CFRelease(cgContext);
        }

        CGColorSpaceRelease(colorSpaceRef);
        CGDataProviderRelease(provider);
        CGImageRelease(iref);
    }
    return noErr;
}