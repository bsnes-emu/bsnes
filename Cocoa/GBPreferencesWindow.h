#import <Cocoa/Cocoa.h>

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource>
@property IBOutlet NSTableView *controlsTableView;
@property IBOutlet NSPopUpButton *graphicsFilterPopupButton;
@end
