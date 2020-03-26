#import "AppDelegate.h"
#include "GBButtons.h"
#include "GBView.h"
#include <Core/gb.h>
#import <Carbon/Carbon.h>

@implementation AppDelegate
{
    NSWindow *preferences_window;
    NSArray<NSView *> *preferences_tabs;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    for (unsigned i = 0; i < GBButtonCount; i++) {
        if ([[defaults objectForKey:button_to_preference_name(i, 0)] isKindOfClass:[NSString class]]) {
            [defaults removeObjectForKey:button_to_preference_name(i, 0)];
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
                                                              @"GBRewind": @(kVK_Tab),
                                                              @"GBSlow-Motion": @(kVK_Shift),

                                                              @"GBFilter": @"NearestNeighbor",
                                                              @"GBColorCorrection": @(GB_COLOR_CORRECTION_EMULATE_HARDWARE),
                                                              @"GBHighpassFilter": @(GB_HIGHPASS_REMOVE_DC_OFFSET),
                                                              @"GBRewindLength": @(10),
                                                              @"GBFrameBlendingMode": @([defaults boolForKey:@"DisableFrameBlending"]? GB_FRAME_BLENDING_MODE_DISABLED : GB_FRAME_BLENDING_MODE_ACCURATE),
                                                              
                                                              @"GBDMGModel": @(GB_MODEL_DMG_B),
                                                              @"GBCGBModel": @(GB_MODEL_CGB_E),
                                                              @"GBSGBModel": @(GB_MODEL_SGB2),
                                                              }];
}

- (IBAction)toggleDeveloperMode:(id)sender
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:![defaults boolForKey:@"DeveloperMode"] forKey:@"DeveloperMode"];
}

- (IBAction)switchPreferencesTab:(id)sender
{
    for (NSView *view in preferences_tabs) {
        [view removeFromSuperview];
    }
    NSView *tab = preferences_tabs[[sender tag]];
    NSRect old = [_preferencesWindow frame];
    NSRect new = [_preferencesWindow frameRectForContentRect:tab.frame];
    new.origin.x = old.origin.x;
    new.origin.y = old.origin.y + (old.size.height - new.size.height);
    [_preferencesWindow setFrame:new display:YES animate:_preferencesWindow.visible];
    [_preferencesWindow.contentView addSubview:tab];
}

- (BOOL)validateMenuItem:(NSMenuItem *)anItem
{
    if ([anItem action] == @selector(toggleDeveloperMode:)) {
        [(NSMenuItem *)anItem setState:[[NSUserDefaults standardUserDefaults] boolForKey:@"DeveloperMode"]];
    }

    return true;
}

- (IBAction) showPreferences: (id) sender
{
    NSArray *objects;
    if (!_preferencesWindow) {
        [[NSBundle mainBundle] loadNibNamed:@"Preferences" owner:self topLevelObjects:&objects];
        NSToolbarItem *first_toolbar_item = [_preferencesWindow.toolbar.items firstObject];
        _preferencesWindow.toolbar.selectedItemIdentifier = [first_toolbar_item itemIdentifier];
        preferences_tabs = @[self.emulationTab, self.graphicsTab, self.audioTab, self.controlsTab];
        [self switchPreferencesTab:first_toolbar_item];
        [_preferencesWindow center];
    }
    [_preferencesWindow makeKeyAndOrderFront:self];
}

- (BOOL)applicationOpenUntitledFile:(NSApplication *)sender
{
    [[NSDocumentController sharedDocumentController] openDocument:self];
    return YES;
}

@end
