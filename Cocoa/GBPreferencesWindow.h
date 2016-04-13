#import <Cocoa/Cocoa.h>

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource>
@property IBOutlet NSTableView *controlsTableView;
@end
