#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate, NSMenuDelegate>

@property (nonatomic, strong) IBOutlet NSWindow *preferencesWindow;
@property (nonatomic, strong) IBOutlet NSView *graphicsTab;
@property (nonatomic, strong) IBOutlet NSView *emulationTab;
@property (nonatomic, strong) IBOutlet NSView *audioTab;
@property (nonatomic, strong) IBOutlet NSView *controlsTab;
- (IBAction)showPreferences: (id) sender;
- (IBAction)toggleDeveloperMode:(id)sender;
- (IBAction)switchPreferencesTab:(id)sender;
@property (nonatomic, weak) IBOutlet NSMenuItem *linkCableMenuItem;

@end

