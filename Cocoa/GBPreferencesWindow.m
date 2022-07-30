#import "GBPreferencesWindow.h"
#import "NSString+StringForKey.h"
#import "GBButtons.h"
#import "BigSurToolbar.h"
#import "GBViewMetal.h"
#import "GBWarningPopover.h"
#import <Carbon/Carbon.h>

@implementation GBPreferencesWindow
{
    bool is_button_being_modified;
    NSInteger button_being_modified;
    signed joystick_configuration_state;
    NSString *joystick_being_configured;
    bool joypad_wait;

    NSPopUpButton *_graphicsFilterPopupButton;
    NSPopUpButton *_highpassFilterPopupButton;
    NSPopUpButton *_colorCorrectionPopupButton;
    NSPopUpButton *_frameBlendingModePopupButton;
    NSPopUpButton *_colorPalettePopupButton;
    NSPopUpButton *_displayBorderPopupButton;
    NSPopUpButton *_rewindPopupButton;
    NSPopUpButton *_rtcPopupButton;
    NSButton *_aspectRatioCheckbox;
    NSButton *_analogControlsCheckbox;
    NSButton *_controllersFocusCheckbox;
    NSEventModifierFlags previousModifiers;
    
    NSPopUpButton *_dmgPopupButton, *_sgbPopupButton, *_cgbPopupButton, *_agbPopupButton;
    NSPopUpButton *_preferredJoypadButton;
    NSPopUpButton *_rumbleModePopupButton;
    NSPopUpButton *_hotkey1PopupButton;
    NSPopUpButton *_hotkey2PopupButton;
    NSSlider *_temperatureSlider;
    NSSlider *_interferenceSlider;
    NSSlider *_volumeSlider;
    NSButton *_autoUpdatesCheckbox;
    NSButton *_OSDCheckbox;
    NSButton *_screenshotFilterCheckbox;
    NSButton *_joystickMBC7Checkbox;
    NSButton *_mouseMBC7Checkbox;
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

- (NSWindowToolbarStyle)toolbarStyle
{
    return NSWindowToolbarStyleExpanded;
}

- (void)close
{
    joystick_configuration_state = -1;
    [self.configureJoypadButton setEnabled:true];
    [self.skipButton setEnabled:false];
    [self.configureJoypadButton setTitle:@"Configure Controller"];
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
    [_colorCorrectionPopupButton selectItemWithTag:mode];
}


- (NSPopUpButton *)colorCorrectionPopupButton
{
    return _colorCorrectionPopupButton;
}

- (void)setTemperatureSlider:(NSSlider *)temperatureSlider
{
    _temperatureSlider = temperatureSlider;
    [temperatureSlider setDoubleValue:[[NSUserDefaults standardUserDefaults] doubleForKey:@"GBLightTemperature"] * 256];
}

- (NSSlider *)temperatureSlider
{
    return _temperatureSlider;
}

- (void)setInterferenceSlider:(NSSlider *)interferenceSlider
{
    _interferenceSlider = interferenceSlider;
    [interferenceSlider setDoubleValue:[[NSUserDefaults standardUserDefaults] doubleForKey:@"GBInterferenceVolume"] * 256];
}

- (NSSlider *)interferenceSlider
{
    return _interferenceSlider;
}

- (void)setVolumeSlider:(NSSlider *)volumeSlider
{
    _volumeSlider = volumeSlider;
    [volumeSlider setDoubleValue:[[NSUserDefaults standardUserDefaults] doubleForKey:@"GBVolume"] * 256];
}

- (NSSlider *)volumeSlider
{
    return _volumeSlider;
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
    [self updatePalettesMenu];
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBColorPalette"];
    if (mode >= 0) {
        [_colorPalettePopupButton selectItemWithTag:mode];
    }
    else {
        [_colorPalettePopupButton selectItemWithTitle:[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"] ?: @""];
    }
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

- (void)setRumbleModePopupButton:(NSPopUpButton *)rumbleModePopupButton
{
    _rumbleModePopupButton = rumbleModePopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBRumbleMode"];
    [_rumbleModePopupButton selectItemWithTag:mode];
}

- (NSPopUpButton *)rumbleModePopupButton
{
    return _rumbleModePopupButton;
}

static inline NSString *keyEquivalentString(NSMenuItem *item)
{
    return [NSString stringWithFormat:@"%s%@", (item.keyEquivalentModifierMask & NSEventModifierFlagShift)? "^":"", item.keyEquivalent];
}

- (void)setHotkey1PopupButton:(NSPopUpButton *)hotkey1PopupButton
{
    _hotkey1PopupButton = hotkey1PopupButton;
    NSString *keyEquivalent = [[NSUserDefaults standardUserDefaults] stringForKey:@"GBJoypadHotkey1"];
    for (NSMenuItem *item in _hotkey1PopupButton.menu.itemArray) {
        if ([keyEquivalent isEqualToString:keyEquivalentString(item)]) {
            [_hotkey1PopupButton selectItem:item];
            break;
        }
    }
}

- (NSPopUpButton *)hotkey1PopupButton
{
    return _hotkey1PopupButton;
}

- (void)setHotkey2PopupButton:(NSPopUpButton *)hotkey2PopupButton
{
    _hotkey2PopupButton = hotkey2PopupButton;
    NSString *keyEquivalent = [[NSUserDefaults standardUserDefaults] stringForKey:@"GBJoypadHotkey2"];
    for (NSMenuItem *item in _hotkey2PopupButton.menu.itemArray) {
        if ([keyEquivalent isEqualToString:keyEquivalentString(item)]) {
            [_hotkey2PopupButton selectItem:item];
            break;
        }
    }
}

- (NSPopUpButton *)hotkey2PopupButton
{
    return _hotkey2PopupButton;
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

- (NSPopUpButton *)rtcPopupButton
{
    return _rtcPopupButton;
}

- (void)setRtcPopupButton:(NSPopUpButton *)rtcPopupButton
{
    _rtcPopupButton = rtcPopupButton;
    NSInteger mode = [[NSUserDefaults standardUserDefaults] integerForKey:@"GBRTCMode"];
    [_rtcPopupButton selectItemAtIndex:mode];
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

- (unsigned) usesForKey:(unsigned) key
{
    unsigned ret = 0;
    for (unsigned player = 4; player--;) {
        for (unsigned button = player == 0? GBButtonCount:GBGameBoyButtonCount; button--;) {
            NSNumber *other = [[NSUserDefaults standardUserDefaults] valueForKey:button_to_preference_name(button, player)];
            if (other && [other unsignedIntValue] == key) {
                ret++;
            }
        }
    }
    return ret;
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
        if ([self usesForKey:[key unsignedIntValue]] > 1) {
            return [[NSAttributedString alloc] initWithString:[NSString displayStringForKeyCode: [key unsignedIntegerValue]]
                                                   attributes:@{NSForegroundColorAttributeName: [NSColor colorWithRed:0.9375 green:0.25 blue:0.25 alpha:1.0],
                                                                NSFontAttributeName: [NSFont boldSystemFontOfSize:[NSFont systemFontSize]]
                                                   }];
        }
        return [NSString displayStringForKeyCode: [key unsignedIntegerValue]];
    }

    return @"";
}

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{

    dispatch_async(dispatch_get_main_queue(), ^{
        is_button_being_modified = true;
        button_being_modified = row;
        tableView.enabled = false;
        self.playerListButton.enabled = false;
        [tableView reloadData];
        [self makeFirstResponder:self];
    });
    return false;
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
    self.controlsTableView.enabled = true;
    self.playerListButton.enabled = true;
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


- (IBAction)changeMBC7JoystickOverride:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] == NSOnState
                                            forKey:@"GBMBC7JoystickOverride"];
}

- (IBAction)changeMBC7AllowMouse:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] == NSOnState
                                            forKey:@"GBMBC7AllowMouse"];
}

- (IBAction)changeAnalogControls:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] == NSOnState
                                            forKey:@"GBAnalogControls"];
}

- (IBAction)changeControllerFocus:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] == NSOnState
                                            forKey:@"GBAllowBackgroundControllers"];
}

- (IBAction)changeAspectRatio:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] != NSOnState
                                            forKey:@"GBAspectRatioUnkept"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBAspectChanged" object:nil];
}

- (IBAction)colorCorrectionChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedItem].tag)
                                              forKey:@"GBColorCorrection"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBColorCorrectionChanged" object:nil];
}

- (IBAction)lightTemperatureChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender doubleValue] / 256.0)
                                              forKey:@"GBLightTemperature"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBLightTemperatureChanged" object:nil];
}

- (IBAction)interferenceVolumeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender doubleValue] / 256.0)
                                              forKey:@"GBInterferenceVolume"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBInterferenceVolumeChanged" object:nil];
}

- (IBAction)volumeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender doubleValue] / 256.0)
                                              forKey:@"GBVolume"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBVolumeChanged" object:nil];
}

- (IBAction)franeBlendingModeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBFrameBlendingMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBFrameBlendingModeChanged" object:nil];
    
}

- (void)updatePalettesMenu
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *themes = [defaults dictionaryForKey:@"GBThemes"];
    NSMenu *menu = _colorPalettePopupButton.menu;
    while (menu.itemArray.count != 4) {
        [menu removeItemAtIndex:4];
    }
    [menu addItem:[NSMenuItem separatorItem]];
    for (NSString *name in [themes.allKeys sortedArrayUsingSelector:@selector(localizedCaseInsensitiveCompare:)]) {
        NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:name action:nil keyEquivalent:@""];
        item.tag = -2;
        [menu addItem:item];
    }
    if (themes) {
        [menu addItem:[NSMenuItem separatorItem]];
    }
    NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:@"Custom…" action:nil keyEquivalent:@""];
    item.tag = -1;
    [menu addItem:item];
}

- (IBAction)colorPaletteChanged:(id)sender
{
    signed tag = [sender selectedItem].tag;
    if (tag == -2) {
        [[NSUserDefaults standardUserDefaults] setObject:@(-1)
                                                  forKey:@"GBColorPalette"];
        [[NSUserDefaults standardUserDefaults] setObject:[sender selectedItem].title
                                                  forKey:@"GBCurrentTheme"];

    }
    else if (tag == -1) {
        [[NSUserDefaults standardUserDefaults] setObject:@(-1)
                                                  forKey:@"GBColorPalette"];
        [_paletteEditorController awakeFromNib];
        [self beginSheet:_paletteEditor completionHandler:^(NSModalResponse returnCode) {
            [self updatePalettesMenu];
            [_colorPalettePopupButton selectItemWithTitle:[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"] ?: @""];
        }];
    }
    else {
        [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedItem].tag)
                                                  forKey:@"GBColorPalette"];
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBColorPaletteChanged" object:nil];
}

- (IBAction)displayBorderChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedItem].tag)
                                              forKey:@"GBBorderMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBBorderModeChanged" object:nil];
}

- (IBAction)rumbleModeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedItem].tag)
                                              forKey:@"GBRumbleMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBRumbleModeChanged" object:nil];
}

- (IBAction)hotkey1Changed:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:keyEquivalentString([sender selectedItem])
                                              forKey:@"GBJoypadHotkey1"];
}

- (IBAction)hotkey2Changed:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:keyEquivalentString([sender selectedItem])
                                              forKey:@"GBJoypadHotkey2"];
}

- (IBAction)rewindLengthChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBRewindLength"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBRewindLengthChanged" object:nil];
}

- (IBAction)rtcModeChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender indexOfSelectedItem])
                                              forKey:@"GBRTCMode"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBRTCModeChanged" object:nil];

}

- (IBAction)changeAutoUpdates:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool: [(NSButton *)sender state] == NSOnState
                                            forKey:@"GBAutoUpdatesEnabled"];
}

- (IBAction) configureJoypad:(id)sender
{
    [self.configureJoypadButton setEnabled:false];
    [self.skipButton setEnabled:true];
    joystick_being_configured = nil;
    [self advanceConfigurationStateMachine];
}

- (IBAction) skipButton:(id)sender
{
    [self advanceConfigurationStateMachine];
}

- (void) advanceConfigurationStateMachine
{
    joystick_configuration_state++;
    if (joystick_configuration_state == GBUnderclock) {
        [self.configureJoypadButton setTitle:@"Press Button for Slo-Mo"]; // Full name is too long :<
    }
    else if (joystick_configuration_state < GBJoypadButtonCount) {
        [self.configureJoypadButton setTitle:[NSString stringWithFormat:@"Press Button for %@", GBButtonNames[joystick_configuration_state]]];
    }
    else {
        joystick_configuration_state = -1;
        [self.configureJoypadButton setEnabled:true];
        [self.skipButton setEnabled:false];
        [self.configureJoypadButton setTitle:@"Configure Joypad"];
    }
}

- (void)controller:(JOYController *)controller buttonChangedState:(JOYButton *)button
{
    /* Debounce */
    if (joypad_wait) return;
    joypad_wait = true;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.25 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        joypad_wait = false;
    });
        
    if (!button.isPressed) return;
    if (joystick_configuration_state == -1) return;
    if (joystick_configuration_state == GBJoypadButtonCount) return;
    if (!joystick_being_configured) {
        joystick_being_configured = controller.uniqueID;
    }
    else if (![joystick_being_configured isEqualToString:controller.uniqueID]) {
        return;
    }
    
    NSMutableDictionary *instance_mappings = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitInstanceMapping"] mutableCopy];
    
    NSMutableDictionary *name_mappings = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitNameMapping"] mutableCopy];

    
    if (!instance_mappings) {
        instance_mappings = [[NSMutableDictionary alloc] init];
    }
    
    if (!name_mappings) {
        name_mappings = [[NSMutableDictionary alloc] init];
    }
    
    NSMutableDictionary *mapping = nil;
    if (joystick_configuration_state != 0) {
        mapping = [instance_mappings[controller.uniqueID] mutableCopy];
    }
    else {
        mapping = [[NSMutableDictionary alloc] init];
    }

    
    static const unsigned gb_to_joykit[] = {
    [GBRight] = JOYButtonUsageDPadRight,
    [GBLeft] = JOYButtonUsageDPadLeft,
    [GBUp] = JOYButtonUsageDPadUp,
    [GBDown] = JOYButtonUsageDPadDown,
    [GBA] = JOYButtonUsageA,
    [GBB] = JOYButtonUsageB,
    [GBSelect] = JOYButtonUsageSelect,
    [GBStart] = JOYButtonUsageStart,
    [GBTurbo] = JOYButtonUsageL1,
    [GBRewind] = JOYButtonUsageL2,
    [GBUnderclock] = JOYButtonUsageR1,
    [GBHotkey1] = GBJoyKitHotkey1,
    [GBHotkey2] = GBJoyKitHotkey2,
    };
    
    if (joystick_configuration_state == GBUnderclock) {
        mapping[@"AnalogUnderclock"] = nil;
        double max = 0;
        for (JOYAxis *axis in controller.axes) {
            if ((axis.value > 0.5 || (axis.equivalentButtonUsage == button.usage)) && axis.value >= max) {
                mapping[@"AnalogUnderclock"] = @(axis.uniqueID);
                break;
            }
        }
    }
    
    if (joystick_configuration_state == GBTurbo) {
        mapping[@"AnalogTurbo"] = nil;
        double max = 0;
        for (JOYAxis *axis in controller.axes) {
            if ((axis.value > 0.5 || (axis.equivalentButtonUsage == button.usage)) && axis.value >= max) {
                max = axis.value;
                mapping[@"AnalogTurbo"] = @(axis.uniqueID);
            }
        }
    }
    
    mapping[n2s(button.uniqueID)] = @(gb_to_joykit[joystick_configuration_state]);
    
    instance_mappings[controller.uniqueID] = mapping;
    name_mappings[controller.deviceName] = mapping;
    [[NSUserDefaults standardUserDefaults] setObject:instance_mappings forKey:@"JoyKitInstanceMapping"];
    [[NSUserDefaults standardUserDefaults] setObject:name_mappings forKey:@"JoyKitNameMapping"];
    [self advanceConfigurationStateMachine];
}

- (NSButton *)joystickMBC7Checkbox
{
    return _joystickMBC7Checkbox;
}

- (void)setJoystickMBC7Checkbox:(NSButton *)joystickMBC7Checkbox
{
    _joystickMBC7Checkbox = joystickMBC7Checkbox;
    [_joystickMBC7Checkbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBMBC7JoystickOverride"]];
}

- (NSButton *)mouseMBC7Checkbox
{
    return _mouseMBC7Checkbox;
}

- (void)setMouseMBC7Checkbox:(NSButton *)mouseMBC7Checkbox
{
    _mouseMBC7Checkbox = mouseMBC7Checkbox;
    [_mouseMBC7Checkbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBMBC7AllowMouse"]];
}

- (NSButton *)analogControlsCheckbox
{
    return _analogControlsCheckbox;
}

- (void)setAnalogControlsCheckbox:(NSButton *)analogControlsCheckbox
{
    _analogControlsCheckbox = analogControlsCheckbox;
    [_analogControlsCheckbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBAnalogControls"]];
}

- (NSButton *)controllersFocusCheckbox
{
    return _controllersFocusCheckbox;
}

- (void)setControllersFocusCheckbox:(NSButton *)controllersFocusCheckbox
{
    _controllersFocusCheckbox = controllersFocusCheckbox;
    [_controllersFocusCheckbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBAllowBackgroundControllers"]];
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
    [JOYController registerListener:self];
    joystick_configuration_state = -1;
}

- (void)dealloc
{
    [JOYController unregisterListener:self];
    [[NSDistributedNotificationCenter defaultCenter] removeObserver:self.controlsTableView];
}

- (IBAction)selectOtherBootROMFolder:(id)sender
{
    NSOpenPanel *panel = [[NSOpenPanel alloc] init];
    [panel setCanChooseDirectories:true];
    [panel setCanChooseFiles:false];
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
        [self.bootROMsFolderItem setHidden:false];
        [self.bootROMsFolderItem setImage:icon];
        [self.bootROMsButton selectItemAtIndex:1];
    }
    else {
        [self.bootROMsFolderItem setHidden:true];
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

- (void)setAgbPopupButton:(NSPopUpButton *)agbPopupButton
{
    _agbPopupButton = agbPopupButton;
    [_agbPopupButton selectItemWithTag:[[NSUserDefaults standardUserDefaults] integerForKey:@"GBAGBModel"]];
}

- (NSPopUpButton *)agbPopupButton
{
    return _agbPopupButton;
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

- (IBAction)agbModelChanged:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setObject:@([sender selectedTag])
                                              forKey:@"GBAGBModel"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"GBAGBModelChanged" object:nil];
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

- (void)controllerConnected:(JOYController *)controller
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.25 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self refreshJoypadMenu:nil];
    });
}

- (void)controllerDisconnected:(JOYController *)controller
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.25 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self refreshJoypadMenu:nil];
    });
}

- (IBAction)refreshJoypadMenu:(id)sender
{
    bool preferred_is_connected = false;
    NSString *player_string = n2s(self.playerListButton.selectedTag);
    NSString *selected_controller = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitDefaultControllers"][player_string];
    
    [self.preferredJoypadButton removeAllItems];
    [self.preferredJoypadButton addItemWithTitle:@"None"];
    for (JOYController *controller in [JOYController allControllers]) {
        [self.preferredJoypadButton addItemWithTitle:[NSString stringWithFormat:@"%@ (%@)", controller.deviceName, controller.uniqueID]];
        
        self.preferredJoypadButton.lastItem.identifier = controller.uniqueID;
        
        if ([controller.uniqueID isEqualToString:selected_controller]) {
            preferred_is_connected = true;
            [self.preferredJoypadButton selectItem:self.preferredJoypadButton.lastItem];
        }
    }
    
    if (!preferred_is_connected && selected_controller) {
        [self.preferredJoypadButton addItemWithTitle:[NSString stringWithFormat:@"Unavailable Controller (%@)", selected_controller]];
        self.preferredJoypadButton.lastItem.identifier = selected_controller;
        [self.preferredJoypadButton selectItem:self.preferredJoypadButton.lastItem];
    }
    

    if (!selected_controller) {
        [self.preferredJoypadButton selectItemWithTitle:@"None"];
    }
    [self.controlsTableView reloadData];
}

- (IBAction)changeDefaultJoypad:(id)sender
{
    NSMutableDictionary *default_joypads = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitDefaultControllers"] mutableCopy];
    if (!default_joypads) {
        default_joypads = [[NSMutableDictionary alloc] init];
    }

    NSString *player_string = n2s(self.playerListButton.selectedTag);
    if ([[sender titleOfSelectedItem] isEqualToString:@"None"]) {
        [default_joypads removeObjectForKey:player_string];
    }
    else {
        default_joypads[player_string] = [[sender selectedItem] identifier];
    }
    [[NSUserDefaults standardUserDefaults] setObject:default_joypads forKey:@"JoyKitDefaultControllers"];
}

- (NSButton *)autoUpdatesCheckbox
{
    return _autoUpdatesCheckbox;
}

- (void)setAutoUpdatesCheckbox:(NSButton *)autoUpdatesCheckbox
{
    _autoUpdatesCheckbox = autoUpdatesCheckbox;
    [_autoUpdatesCheckbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBAutoUpdatesEnabled"]];
}

- (NSButton *)OSDCheckbox
{
    return _OSDCheckbox;
}

- (void)setOSDCheckbox:(NSButton *)OSDCheckbox
{
    _OSDCheckbox = OSDCheckbox;
    [_OSDCheckbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBOSDEnabled"]];
}

- (IBAction)changeOSDEnabled:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool:[(NSButton *)sender state] == NSOnState
                                            forKey:@"GBOSDEnabled"];

}

- (IBAction)changeFilterScreenshots:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setBool:[(NSButton *)sender state] == NSOnState
                                            forKey:@"GBFilterScreenshots"];
}

- (NSButton *)screenshotFilterCheckbox
{
    return _screenshotFilterCheckbox;
}

- (void)setScreenshotFilterCheckbox:(NSButton *)screenshotFilterCheckbox
{
    _screenshotFilterCheckbox = screenshotFilterCheckbox;
    if (![GBViewMetal isSupported]) {
        [_screenshotFilterCheckbox setEnabled:false];
    }
    else {
        [_screenshotFilterCheckbox setState: [[NSUserDefaults standardUserDefaults] boolForKey:@"GBFilterScreenshots"]];
    }
}

- (IBAction)displayColorCorrectionHelp:(id)sender
{
    [GBWarningPopover popoverWithContents:
         (NSString * const[]){
            [GB_COLOR_CORRECTION_DISABLED] = @"Colors are directly interpreted as sRGB, resulting in unbalanced colors and inaccurate hues.",
            [GB_COLOR_CORRECTION_CORRECT_CURVES] = @"Colors have their brightness corrected, but hues remain unbalanced.",
            [GB_COLOR_CORRECTION_MODERN_BALANCED] = @"Emulates a modern display. Blue contrast is moderately enhanced at the cost of slight hue inaccuracy.",
            [GB_COLOR_CORRECTION_MODERN_BOOST_CONTRAST] = @"Like Modern – Balanced, but further boosts the contrast of greens and magentas that is lacking on the original hardware.",
            [GB_COLOR_CORRECTION_REDUCE_CONTRAST] = @"Slightly reduce the contrast to better represent the tint and contrast of the original display.",
            [GB_COLOR_CORRECTION_LOW_CONTRAST] = @"Harshly reduce the contrast to accurately represent the tint low constrast of the original display.",
            [GB_COLOR_CORRECTION_MODERN_ACCURATE] = @"Emulates a modern display. Colors have their hues and brightness corrected.",
         } [self.colorCorrectionPopupButton.selectedItem.tag]
                                   title:self.colorCorrectionPopupButton.selectedItem.title
                                   onView:sender
                                  timeout:6
                            preferredEdge:NSRectEdgeMaxX];
}

- (IBAction)displayHighPassHelp:(id)sender
{
    [GBWarningPopover popoverWithContents:
     (NSString * const[]){
        [GB_HIGHPASS_OFF] = @"No high-pass filter will be applied. DC offset will be kept, pausing and resuming will trigger audio pops.",
        [GB_HIGHPASS_ACCURATE] = @"An accurate high-pass filter will be applied, removing the DC offset while somewhat attenuating the bass.",
        [GB_HIGHPASS_REMOVE_DC_OFFSET] = @"A high-pass filter will be applied to the DC offset itself, removing the DC offset while preserving the waveform.",
    } [self.highpassFilterPopupButton.indexOfSelectedItem]
                                    title:self.highpassFilterPopupButton.selectedItem.title
                                   onView:sender
                                  timeout:6
                            preferredEdge:NSRectEdgeMaxX];
}

@end
