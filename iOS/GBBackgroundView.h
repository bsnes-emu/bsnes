#import <UIKit/UIKit.h>
#import "GBLayout.h"
#import "GBView.h"

@interface GBBackgroundView : UIImageView
@property (readonly) GBView *gbView;
@property (nonatomic) GBLayout *layout;
@property (nonatomic) bool usesSwipePad;
@end
