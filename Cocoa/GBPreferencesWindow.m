#import "GBPreferencesWindow.h"
#import "NSString+StringForKey.h"
#import "GBButtons.h"
#import <Carbon/Carbon.h>

@implementation GBPreferencesWindow
{
    bool is_button_being_modified;
    NSInteger button_being_modified;
    signed joystick_configuration_state;
    NSString *joystick_being_configured;
    signed last_axis;

    NSPopUpButton *_graphicsFilterPopupButton;
    NSPopUpButton *_highpassFilterPopupButton;
    NSPopUpButton *_colorCorrectionPopupButton;
    NSPopUpButton *_rewindPopupButton;
    NSButton *_aspectRatioCheckbox;
    NSEventModifierFlags previousModifiers;
}

+ (NSArray *)filterList
{
    /* The filter list as ordered in the popup button */
    static NSArray * filters = nil;
    if (!filters) {
        filters = @[
                    @"NearestNeighbor",
                    @"Bilinear",
                    @"SmoothBilinear",
                    @"LCD",
                    @"Scale2x",
                    @"Scale4x",
                    @"AAScale2x",
                    @"AAScale4x",
                    @"HQ2x",
                    @"OmniScale",
                    @"OmniScaleLegacy",
                    @"AAOmniScaleLegacy",
                    ];
    }
    return filters;
}

- (void)close
{
    joystick_configuration_state = -1;
    [self.configureJoypadButton setEnabled:YES];
    [self.skipButton setEnabled:NO];
    [self.configureJoypadButton setTitle:@"Configure Joypad"];
    [super close];
}

- (NSPopUpButton *)graphicsFilterPopupButton
{
    return _graphicsFilterPopupButton;
}

- (void)setGraphicsFilterPopupButton:(NSPopUpButton *)graphicsFilterPopupButton
{
    _graphicsFilterPopupButton = graphicsFilterPopupButton;
    NSString *filter = [[NSUserDefaults standardUserDefaults] objectForKey:@"GBFilter"];
    [_graphicsFilterPopupButton selectItemAtIndex:[[[self class] filterList] indexOfObject:filter]];
}

- (NSPopUpButton *)highpassFilterPopupButton
{
    return _highpassFilterPopupButton;
}

- (void)setColorCorrectionPopupButton:(NSPopUpButton *)colorCorrectionPopupButton
{
    _colorCorrectionPopupButton = colorCorrectionPopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBColorCorrection"];
    [_colorCorrectionPopupButton selectItemAtIndex:mode];
}

- (NSPopUpButton *)colorCorrectionPopupButton
{
    return _colorCorrectionPopupButton;
}

- (void)setRewindPopupButton:(NSPopUpButton *)rewindPopupButton
{
    _rewindPopupButton = rewindPopupButton;
    NSInteger length = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBRewindLength"];
    [_rewindPopupButton selectItemWithTag:length];
}

- (NSPopUpButton *)rewindPopupButton
{
    return _rewindPopupButton;
}

- (void)setHighpassFilterPopupButton:(NSPopUpButton *)highpassFilterPopupButton
{
    _highpassFilterPopupButton = highpassFilterPopupButton;
    [_highpassFilterPopupButton selectItemAtIndex:[[[NSUserDefaults standardUserDefaults] objectForKey:@"GBHighpassFilter"] unsignedIntegerValue]];
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

    return [NSString displayStringForKeyCode:[[NSUserDefaults standardUserDefaults] integerForKey:
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
        if (self.firstResponder != self.controlsTableView && [theEvent type] != NSEventTypeFlagsChanged) {
            [super keyDown:theEvent];
        }
        return;
    }

    is_button_being_modified = false;

    [[NSUserDefaults standardUserDefaults] setInteger:theEvent.keyCode
                                              forKey:button_to_preference_name(button_being_modified)];
    self.controlsTableView.enabled = YES;
    [self.controlsTableView reloadData];
    [self makeFirstResponder:self.controlsTableView];
}

- (void) flagsChanged:(NSEvent *)event
{
    if (event.modifierFlags > previousModifiers) {
        [self keyDown:event];
    }
    
    previousModifiers = event.modifierFlags;
}

- (IBAction)graphicFilterChanged:(NSPopUpButton *)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:[[self class] filterList][[sender indexOfSelectedItem]]
                                              forKey:@"GBFilter"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBFilterChanged" object:nil];
}

- (IBAction)highpassFilterChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBHighpassFilter"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBHighpassFilterChanged" object:nil];
}

- (IBAction)changeAspectRatio:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] != NSOnState
                                            forKey:@"GBAspectRatioUnkept"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBAspectChanged" object:nil];
}

- (IBAction)colorCorrectionChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBColorCorrection"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBColorCorrectionChanged" object:nil];

}

- (IBAction)rewindLengthChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBRewindLength"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBRewindLengthChanged" object:nil];
}

- (IBAction) configureJoypad:(id)sender
{
    [self.configureJoypadButton setEnabled:NO];
    [self.skipButton setEnabled:YES];
    joystick_being_configured = nil;
    [self advanceConfigurationStateMachine];
    last_axis = -1;
}

- (IBAction) skipButton:(id)sender
{
    [self advanceConfigurationStateMachine];
}

- (void) advanceConfigurationStateMachine
{
    joystick_configuration_state++;
    if (joystick_configuration_state < GBButtonCount) {
        [self.configureJoypadButton setTitle:[NSString stringWithFormat:@"Press Button for %@", GBButtonNames[joystick_configuration_state]]];
    }
    else if (joystick_configuration_state == GBButtonCount) {
        [self.configureJoypadButton setTitle:@"Move the Analog Stick"];
    }
    else {
        joystick_configuration_state = -1;
        [self.configureJoypadButton setEnabled:YES];
        [self.skipButton setEnabled:NO];
        [self.configureJoypadButton setTitle:@"Configure Joypad"];
    }
}

- (void) joystick:(NSString *)joystick_name button: (unsigned)button changedState: (bool) state
{
    if (!state) return;
    if (joystick_configuration_state == -1) return;
    if (joystick_configuration_state == GBButtonCount) return;
    if (!joystick_being_configured) {
        joystick_being_configured = joystick_name;
    }
    else if (![joystick_being_configured isEqualToString:joystick_name]) {
        return;
    }
    
    NSMutableDictionary *all_mappings = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBJoypadMappings"] mutableCopy];
    
    if (!all_mappings) {
        all_mappings = [[NSMutableDictionary alloc] init];
    }
    
    NSMutableDictionary *mapping = [[all_mappings objectForKey:joystick_name] mutableCopy];
    
    if (!mapping) {
        mapping = [[NSMutableDictionary alloc] init];
    }
    
    mapping[GBButtonNames[joystick_configuration_state]] = @(button);
    
    all_mappings[joystick_name] = mapping;
    [[NSUserDefaults standardUserDefaults] setObject:all_mappings forKey:@"GBJoypadMappings"];
    [self advanceConfigurationStateMachine];
}

- (void) joystick:(NSString *)joystick_name axis: (unsigned)axis movedTo: (signed) value
{
    if (abs(value) < 0x4000) return;
    if (joystick_configuration_state != GBButtonCount) return;
    if (!joystick_being_configured) {
        joystick_being_configured = joystick_name;
    }
    else if (![joystick_being_configured isEqualToString:joystick_name]) {
        return;
    }
    
    if (last_axis == -1) {
        last_axis = axis;
        return;
    }
    
    if (axis == last_axis) {
        return;
    }
    
    NSMutableDictionary *all_mappings = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBJoypadMappings"] mutableCopy];
    
    if (!all_mappings) {
        all_mappings = [[NSMutableDictionary alloc] init];
    }
    
    NSMutableDictionary *mapping = [[all_mappings objectForKey:joystick_name] mutableCopy];
    
    if (!mapping) {
        mapping = [[NSMutableDictionary alloc] init];
    }
    
    mapping[@"XAxis"] = @(MIN(axis, last_axis));
    mapping[@"YAxis"] = @(MAX(axis, last_axis));
    
    all_mappings[joystick_name] = mapping;
    [[NSUserDefaults standardUserDefaults] setObject:all_mappings forKey:@"GBJoypadMappings"];
    [self advanceConfigurationStateMachine];
}

- (NSButton *)aspectRatioCheckbox
{
    return _aspectRatioCheckbox;
}

- (void)setAspectRatioCheckbox:(NSButton *)aspectRatioCheckbox
{
    _aspectRatioCheckbox = aspectRatioCheckbox;
    [_aspectRatioCheckbox setState: ![[NSUserDefaults standardUserDefaults] boolForKey:@"GBAspectRatioUnkept"]];
}

- (void)awakeFromNib
{
    [super awakeFromNib];
    [[NSDistributedNotificationCenter defaultCenter] addObserver:self.controlsTableView selector:@selector(reloadData) name:(NSString*)kTISNotifySelectedKeyboardInputSourceChanged object:nil];
}

- (void)dealloc
{
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:self.controlsTableView];
}

@end
