#import <UIKit/UIKit.h>

@interface GBTheme : NSObject
@property (readonly, direct) UIColor *brandColor;
@property (readonly, direct) UIColor *backgroundGradientTop;
@property (readonly, direct) UIColor *backgroundGradientBottom;
@property (readonly, direct) UIColor *bezelsGradientTop;
@property (readonly, direct) UIColor *bezelsGradientBottom;


@property (readonly, direct) NSString *name;

@property (readonly, direct) bool renderingPreview; // Kind of a hack

@property (readonly, direct) UIImage *horizontalPreview;
@property (readonly, direct) UIImage *verticalPreview;

@property (readonly, direct) bool isDark;

- (instancetype)initDefaultTheme __attribute__((objc_direct));
- (instancetype)initDarkTheme __attribute__((objc_direct));


- (UIImage *)imageNamed:(NSString *)name  __attribute__((objc_direct));

@end
