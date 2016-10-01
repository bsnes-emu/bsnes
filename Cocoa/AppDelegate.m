#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate
{
    NSWindow *preferences_window;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
#define KEY(x) ({unichar __x = x; [NSString stringWithCharacters:&(__x) length:1];})
    [[NSUserDefaults standardUserDefaults] registerDefaults:@{
                                                              @"GBRight": KEY(NSRightArrowFunctionKey),
                                                              @"GBLeft": KEY(NSLeftArrowFunctionKey),
                                                              @"GBUp": KEY(NSUpArrowFunctionKey),
                                                              @"GBDown": KEY(NSDownArrowFunctionKey),

                                                              @"GBA": @"x",
                                                              @"GBB": @"z",
                                                              @"GBSelect": @"\x7f",
                                                              @"GBStart": @"\r",

                                                              @"GBTurbo": @" ",

                                                              @"GBFilter": @"NearestNeighbor",
                                                              }];
#undef KEY
}

- (IBAction)toggleDeveloperMode:(id)sender {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:![defaults boolForKey:@"DeveloperMode"] forKey:@"DeveloperMode"];
}

- (BOOL)validateMenuItem:(NSMenuItem *)anItem
{
    if ([anItem action] == @selector(toggleDeveloperMode:)) {
        [(NSMenuItem*)anItem setState:[[NSUserDefaults standardUserDefaults] boolForKey:@"DeveloperMode"]];
    }

    return true;
}

- (IBAction) showPreferences: (id) sender
{
    NSArray *objects;
    if (!_preferencesWindow) {
        [[NSBundle mainBundle] loadNibNamed:@"Preferences" owner:self topLevelObjects:&objects];
    }
    [_preferencesWindow makeKeyAndOrderFront:self];
}

- (BOOL)applicationOpenUntitledFile:(NSApplication *)sender
{
    [[NSDocumentController sharedDocumentController] openDocument:self];
    return YES;
}

@end
