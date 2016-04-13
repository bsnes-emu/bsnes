#import "GBPreferencesWindow.h"
#import "NSString+StringForKey.h"
#import "GBButtons.h"

@implementation GBPreferencesWindow
{
    bool is_button_being_modified;
    NSInteger button_being_modified;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return GBButtonCount;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    if ([tableColumn.identifier isEqualToString:@"keyName"]) {
        return GBButtonNames[row];
    }

    if (is_button_being_modified && button_being_modified == row) {
        return @"Select a new key...";
    }

    return [NSString displayStringForKeyString:[[NSUserDefaults standardUserDefaults] stringForKey:
                                                button_to_preference_name(row)]];
}

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    dispatch_async(dispatch_get_main_queue(), ^{
        is_button_being_modified = true;
        button_being_modified = row;
        tableView.enabled = NO;
        [tableView reloadData];
        [self makeFirstResponder:self];
    });
    return NO;
}

-(void)keyDown:(NSEvent *)theEvent
{
    if (!is_button_being_modified) {
        if (self.firstResponder != self.controlsTableView) {
            [super keyDown:theEvent];
        }
        return;
    }

    is_button_being_modified = false;

    [[NSUserDefaults standardUserDefaults] setObject:theEvent.charactersIgnoringModifiers
                                              forKey:button_to_preference_name(button_being_modified)];
    self.controlsTableView.enabled = YES;
    [self.controlsTableView reloadData];
    [self makeFirstResponder:self.controlsTableView];
}

@end
