#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate, NSMenuDelegate>

@property IBOutlet NSWindow *preferencesWindow;
@property (strong) IBOutlet NSView *graphicsTab;
@property (strong) IBOutlet NSView *emulationTab;
@property (strong) IBOutlet NSView *audioTab;
@property (strong) IBOutlet NSView *controlsTab;
- (IBAction)showPreferences: (id) sender;
- (IBAction)toggleDeveloperMode:(id)sender;
- (IBAction)switchPreferencesTab:(id)sender;
@property (weak) IBOutlet NSMenuItem *linkCableMenuItem;

@end

