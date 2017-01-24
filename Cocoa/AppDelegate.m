#import "AppDelegate.h"
#include "GBButtons.h"
#import <Carbon/Carbon.h>

@implementation AppDelegate
{
    NSWindow *preferences_window;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    for (unsigned i = 0; i < GBButtonCount; i++) {
        if ([[defaults objectForKey:button_to_preference_name(i)] isKindOfClass:[NSString class]]) {
            [defaults removeObjectForKey:button_to_preference_name(i)];
        }
    }
    [[NSUserDefaults standardUserDefaults] registerDefaults:@{
                                                              @"GBRight": @(kVK_RightArrow),
                                                              @"GBLeft": @(kVK_LeftArrow),
                                                              @"GBUp": @(kVK_UpArrow),
                                                              @"GBDown": @(kVK_DownArrow),

                                                              @"GBA": @(kVK_ANSI_X),
                                                              @"GBB": @(kVK_ANSI_Z),
                                                              @"GBSelect": @(kVK_Delete),
                                                              @"GBStart": @(kVK_Return),

                                                              @"GBTurbo": @(kVK_Space),

                                                              @"GBFilter": @"NearestNeighbor",
                                                              }];
}

- (IBAction)toggleDeveloperMode:(id)sender
{
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
