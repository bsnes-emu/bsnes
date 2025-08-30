#import <UIKit/UIKit.h>

@interface GBROMViewController : UITableViewController<UIDocumentPickerDelegate>

/* For inheritance */
- (void)romSelectedAtIndex:(unsigned)index;
- (void)deleteROMAtIndex:(unsigned)index;
- (void)renameROM:(NSString *)oldName toName:(NSString *)newName;
- (void)duplicateROMAtIndex:(unsigned)index;
- (NSString *)rootPath;

/* To be used by subclasses */
- (UITableViewCell *)cellForROM:(NSString *)rom;
@end
