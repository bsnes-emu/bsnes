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
    NSPopUpButton *_frameBlendingModePopupButton;
    NSPopUpButton *_colorPalettePopupButton;
    NSPopUpButton *_displayBorderPopupButton;
    NSPopUpButton *_rewindPopupButton;
    NSButton *_aspectRatioCheckbox;
    NSEventModifierFlags previousModifiers;
    
    NSPopUpButton *_dmgPopupButton, *_sgbPopupButton, *_cgbPopupButton;
    NSPopUpButton *_preferredJoypadButton;
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
                    @"MonoLCD",
                    @"LCD",
                    @"CRT",
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

- (void)setFrameBlendingModePopupButton:(NSPopUpButton *)frameBlendingModePopupButton
{
    _frameBlendingModePopupButton = frameBlendingModePopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBFrameBlendingMode"];
    [_frameBlendingModePopupButton selectItemAtIndex:mode];
}

- (NSPopUpButton *)frameBlendingModePopupButton
{
    return _frameBlendingModePopupButton;
}

- (void)setColorPalettePopupButton:(NSPopUpButton *)colorPalettePopupButton
{
    _colorPalettePopupButton = colorPalettePopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBColorPalette"];
    [_colorPalettePopupButton selectItemAtIndex:mode];
}

- (NSPopUpButton *)colorPalettePopupButton
{
    return _colorPalettePopupButton;
}

- (void)setDisplayBorderPopupButton:(NSPopUpButton *)displayBorderPopupButton
{
    _displayBorderPopupButton = displayBorderPopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBBorderMode"];
    [_displayBorderPopupButton selectItemWithTag:mode];
}

- (NSPopUpButton *)displayBorderPopupButton
{
    return _displayBorderPopupButton;
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
    if (self.playerListButton.selectedTag == 0) {
        return GBButtonCount;
    }
    return GBGameBoyButtonCount;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    if ([tableColumn.identifier isEqualToString:@"keyName"]) {
        return GBButtonNames[row];
    }

    if (is_button_being_modified && button_being_modified == row) {
        return @"Select a new key...";
    }
    
    NSNumber *key = [[NSUserDefaults standardUserDefaults] valueForKey:button_to_preference_name(row, self.playerListButton.selectedTag)];
    if (key) {
        return [NSString displayStringForKeyCode: [key unsignedIntegerValue]];
    }

    return @"";
}

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{

    dispatch_async(dispatch_get_main_queue(), ^{
        is_button_being_modified = true;
        button_being_modified = row;
        tableView.enabled = NO;
        self.playerListButton.enabled = NO;
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
                                              forKey:button_to_preference_name(button_being_modified, self.playerListButton.selectedTag)];
    self.controlsTableView.enabled = YES;
    self.playerListButton.enabled = YES;
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

- (IBAction)franeBlendingModeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBFrameBlendingMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBFrameBlendingModeChanged" object:nil];
    
}

- (IBAction)colorPaletteChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBColorPalette"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBColorPaletteChanged" object:nil];
}

- (IBAction)displayBorderChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedItem].tag)
                                              forKey:@"GBBorderMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBBorderModeChanged" object:nil];
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
    [self refreshJoypadMenu:nil];
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

- (void) joystick:(NSString *)joystick_name hat: (unsigned)hat changedState: (int8_t) state
{
    /* Hats are always mapped to the D-pad, ignore them on non-Dpad keys and skip the D-pad configuration if used*/
    if (!state) return;
    if (joystick_configuration_state == -1) return;
    if (joystick_configuration_state > GBDown) return;
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
    
    for (joystick_configuration_state = 0;; joystick_configuration_state++) {
        [mapping removeObjectForKey:GBButtonNames[joystick_configuration_state]];
        if (joystick_configuration_state == GBDown) break;
    }
    
    all_mappings[joystick_name] = mapping;
    [[NSUserDefaults standardUserDefaults] setObject:all_mappings forKey:@"GBJoypadMappings"];
    [self refreshJoypadMenu:nil];
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
    [self updateBootROMFolderButton];
    [[NSDistributedNotificationCenter defaultCenter] addObserver:self.controlsTableView selector:@selector(reloadData) name:(NSString*)kTISNotifySelectedKeyboardInputSourceChanged object:nil];
}

- (void)dealloc
{
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:self.controlsTableView];
}

- (IBAction)selectOtherBootROMFolder:(id)sender
{
    NSOpenPanel *panel = [[NSOpenPanel alloc] init];
    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:NO];
    [panel setPrompt:@"Select"];
    [panel setDirectoryURL:[[NSUserDefaults standardUserDefaults] URLForKey:@"GBBootROMsFolder"]];
    [panel beginSheetModalForWindow:self completionHandler:^(NSModalResponse result) {
        if (result == NSModalResponseOK) {
            NSURL *url = [[panel URLs] firstObject];
            [[NSUserDefaults standardUserDefaults] setURL:url forKey:@"GBBootROMsFolder"];
        }
        [self updateBootROMFolderButton];
    }];

}

- (void) updateBootROMFolderButton
{
    NSURL *url = [[NSUserDefaults standardUserDefaults] URLForKey:@"GBBootROMsFolder"];
    BOOL is_dir = false;
    [[NSFileManager defaultManager] fileExistsAtPath:[url path] isDirectory:&is_dir];
    if (!is_dir) url = nil;
    
    if (url) {
        [self.bootROMsFolderItem setTitle:[url lastPathComponent]];
        NSImage *icon = [[NSWorkspace sharedWorkspace] iconForFile:[url path]];
        [icon setSize:NSMakeSize(16, 16)];
        [self.bootROMsFolderItem setHidden:NO];
        [self.bootROMsFolderItem setImage:icon];
        [self.bootROMsButton selectItemAtIndex:1];
    }
    else {
        [self.bootROMsFolderItem setHidden:YES];
        [self.bootROMsButton selectItemAtIndex:0];
    }
}

- (IBAction)useBuiltinBootROMs:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setURL:nil forKey:@"GBBootROMsFolder"];
    [self updateBootROMFolderButton];
}

- (void)setDmgPopupButton:(NSPopUpButton *)dmgPopupButton
{
    _dmgPopupButton = dmgPopupButton;
    [_dmgPopupButton selectItemWithTag:[[NSUserDefaults standardUserDefaults] integerForKey:@"GBDMGModel"]];
}

- (NSPopUpButton *)dmgPopupButton
{
    return _dmgPopupButton;
}

- (void)setSgbPopupButton:(NSPopUpButton *)sgbPopupButton
{
    _sgbPopupButton = sgbPopupButton;
    [_sgbPopupButton selectItemWithTag:[[NSUserDefaults standardUserDefaults] integerForKey:@"GBSGBModel"]];
}

- (NSPopUpButton *)sgbPopupButton
{
    return _sgbPopupButton;
}

- (void)setCgbPopupButton:(NSPopUpButton *)cgbPopupButton
{
    _cgbPopupButton = cgbPopupButton;
    [_cgbPopupButton selectItemWithTag:[[NSUserDefaults standardUserDefaults] integerForKey:@"GBCGBModel"]];
}

- (NSPopUpButton *)cgbPopupButton
{
    return _cgbPopupButton;
}

- (IBAction)dmgModelChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBDMGModel"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBDMGModelChanged" object:nil];

}

- (IBAction)sgbModelChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBSGBModel"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBSGBModelChanged" object:nil];
}

- (IBAction)cgbModelChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBCGBModel"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBCGBModelChanged" object:nil];
}

- (IBAction)reloadButtonsData:(id)sender
{
    [self.controlsTableView reloadData];
}

- (void)setPreferredJoypadButton:(NSPopUpButton *)preferredJoypadButton
{
    _preferredJoypadButton = preferredJoypadButton;
    [self refreshJoypadMenu:nil];
}

- (NSPopUpButton *)preferredJoypadButton
{
    return _preferredJoypadButton;
}

- (IBAction)refreshJoypadMenu:(id)sender
{
    NSArray *joypads = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBJoypadMappings"] allKeys];
    for (NSString *joypad in joypads) {
        if ([self.preferredJoypadButton indexOfItemWithTitle:joypad] == -1) {
            [self.preferredJoypadButton addItemWithTitle:joypad];
        }
    }
    
    NSString *player_string = [NSString stringWithFormat: @"%ld", (long)self.playerListButton.selectedTag];
    NSString *selected_joypad = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBDefaultJoypads"][player_string];
    if (selected_joypad && [self.preferredJoypadButton indexOfItemWithTitle:selected_joypad] != -1) {
        [self.preferredJoypadButton selectItemWithTitle:selected_joypad];
    }
    else {
        [self.preferredJoypadButton selectItemWithTitle:@"None"];
    }
    [self.controlsTableView reloadData];
}

- (IBAction)changeDefaultJoypad:(id)sender
{
    NSMutableDictionary *default_joypads = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBDefaultJoypads"] mutableCopy];
    if (!default_joypads) {
        default_joypads = [[NSMutableDictionary alloc] init];
    }

    NSString *player_string = [NSString stringWithFormat: @"%ld", self.playerListButton.selectedTag];
    if ([[sender titleOfSelectedItem] isEqualToString:@"None"]) {
        [default_joypads removeObjectForKey:player_string];
    }
    else {
        default_joypads[player_string] = [sender titleOfSelectedItem];
    }
    [[NSUserDefaults standardUserDefaults] setObject:default_joypads forKey:@"GBDefaultJoypads"];
}
@end
