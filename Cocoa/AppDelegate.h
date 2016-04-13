#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property IBOutlet NSWindow *preferencesWindow;
- (IBAction)showPreferences: (id) sender;
- (IBAction)toggleDeveloperMode:(id)sender;

@end

