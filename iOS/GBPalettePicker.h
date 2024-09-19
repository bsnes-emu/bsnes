#import <UIKit/UIKit.h>
#import <Core/gb.h>

@interface GBPalettePicker : UITableViewController
+ (const GB_palette_t *)paletteForTheme:(NSString *)theme;
@end

