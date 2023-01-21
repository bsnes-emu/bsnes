#import <UIKit/UIKit.h>
#include <Core/gb.h>

@interface GBSettingsViewController : UITableViewController
+ (UIViewController *)settingsViewControllerWithLeftButton:(UIBarButtonItem *)button;
+ (const GB_palette_t *)paletteForTheme:(NSString *)theme;
@end
