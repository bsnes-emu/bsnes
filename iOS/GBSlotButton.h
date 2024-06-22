#import <UIKit/UIKit.h>

@interface GBSlotButton : UIButton
+ (instancetype)buttonWithLabelText:(NSString *)label;
@property (readonly) UILabel *label;
@property (readonly) UILabel *slotSubtitleLabel;
@property (nonatomic, getter=isShowingMenu) bool showingMenu;
@end
