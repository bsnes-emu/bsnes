#import <UIKit/UIKit.h>

@interface GBLayout : NSObject
@property (readonly) UIImage *background;
@property (readonly) CGRect screenRect;
@property (readonly) CGPoint dpadLocation;
@property (readonly) CGPoint aLocation;
@property (readonly) CGPoint bLocation;
@property (readonly) CGPoint startLocation;
@property (readonly) CGPoint selectLocation;
- (CGRect)viewRectForOrientation:(UIInterfaceOrientation)orientation;
@end

#ifdef GBLayoutInternal

@interface GBLayout()
@property UIImage *background;
@property CGRect screenRect;
@property CGPoint dpadLocation;
@property CGPoint aLocation;
@property CGPoint bLocation;
@property CGPoint startLocation;
@property CGPoint selectLocation;
@property (readonly) CGSize resolution;
@property (readonly) unsigned factor;
@property (readonly) unsigned minY;
@property (readonly) unsigned homeBar;
@property (readonly) unsigned cutout;
@property (readonly) bool hasFractionalPixels;

@property (readonly) UIColor *brandColor;

- (void)drawBackground;
- (void)drawScreenBezels;
- (void)drawLogoInVerticalRange:(NSRange)range;
- (void)drawLabels;

- (CGSize)buttonDeltaForMaxHorizontalDistance:(double)distance;
@end

#endif
