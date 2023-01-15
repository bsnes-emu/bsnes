#import <UIKit/UIKit.h>

@interface GBViewController : UIViewController <UIApplicationDelegate>
@property (nullable, nonatomic, strong) UIWindow *window;
- (void)reset;
- (void)openLibrary;
- (void)start;
- (void)stop;
- (void)changeModel;
@end
