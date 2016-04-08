#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (IBAction)toggleDeveloperMode:(id)sender {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:![defaults boolForKey:@"DeveloperMode"] forKey:@"DeveloperMode"];
}

- (BOOL)validateMenuItem:(NSMenuItem *)anItem
{
    if ([anItem action] == @selector(toggleDeveloperMode:)) {
        [(NSMenuItem*)anItem setState:[[NSUserDefaults standardUserDefaults] boolForKey:@"DeveloperMode"]];
        return true;
    }

    return false;
}

@end
