#import <Cocoa/Cocoa.h>

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource>
@property IBOutlet NSTableView *controlsTableView;
@property IBOutlet NSPopUpButton *graphicsFilterPopupButton;
@property (strong) IBOutlet NSButton *aspectRatioCheckbox;
@property (strong) IBOutlet NSPopUpButton *highpassFilterPopupButton;
@property (strong) IBOutlet NSPopUpButton *colorCorrectionPopupButton;
@end
