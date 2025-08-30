#import <Cocoa/Cocoa.h>

@interface NSSlider (GBHueSlider)
-(NSColor *)colorValue;
@end

@interface GBHueSliderCell : NSSliderCell
-(NSColor *)colorValue;
@end
