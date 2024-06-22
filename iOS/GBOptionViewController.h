#import <UIKit/UIKit.h>

@interface GBOptionViewController : UITableViewController
- (instancetype)initWithHeader:(NSString *)header;
@property NSString *header;
@property NSString *footer;
@property (getter=isModal) bool modal;
- (void)addOption:(NSString *)title
    withCheckmark:(bool)checked
           action:(void (^)(void))block;
@end
