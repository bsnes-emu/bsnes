#import <Cocoa/Cocoa.h>
#import "GBJoystickListener.h"

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource, GBJoystickListener>
@property IBOutlet NSTableView *controlsTableView;
@property IBOutlet NSPopUpButton *graphicsFilterPopupButton;
@property (strong) IBOutlet NSButton *aspectRatioCheckbox;
@property (strong) IBOutlet NSPopUpButton *highpassFilterPopupButton;
@property (strong) IBOutlet NSPopUpButton *colorCorrectionPopupButton;
@property (strong) IBOutlet NSPopUpButton *rewindPopupButton;
@property (strong) IBOutlet NSButton *configureJoypadButton;
@property (strong) IBOutlet NSButton *skipButton;
@property (strong) IBOutlet NSMenuItem *bootROMsFolderItem;
@property (strong) IBOutlet NSPopUpButtonCell *bootROMsButton;

@property (weak) IBOutlet NSPopUpButton *dmgPopupButton;
@property (weak) IBOutlet NSPopUpButton *sgbPopupButton;
@property (weak) IBOutlet NSPopUpButton *cgbPopupButton;

@end
