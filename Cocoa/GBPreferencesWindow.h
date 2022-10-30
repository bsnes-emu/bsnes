#import <Cocoa/Cocoa.h>
#import <JoyKit/JoyKit.h>
#import "GBPaletteEditorController.h"

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource, JOYListener>
@property IBOutlet NSTableView *controlsTableView;
@property IBOutlet NSPopUpButton *graphicsFilterPopupButton;
@property IBOutlet NSButton *analogControlsCheckbox;
@property IBOutlet NSButton *controllersFocusCheckbox;
@property IBOutlet NSButton *aspectRatioCheckbox;
@property IBOutlet NSPopUpButton *highpassFilterPopupButton;
@property IBOutlet NSPopUpButton *colorCorrectionPopupButton;
@property IBOutlet NSPopUpButton *frameBlendingModePopupButton;
@property IBOutlet NSPopUpButton *colorPalettePopupButton;
@property IBOutlet NSPopUpButton *displayBorderPopupButton;
@property IBOutlet NSPopUpButton *rewindPopupButton;
@property IBOutlet NSPopUpButton *rtcPopupButton;
@property IBOutlet NSButton *configureJoypadButton;
@property IBOutlet NSButton *skipButton;
@property IBOutlet NSMenuItem *bootROMsFolderItem;
@property IBOutlet NSPopUpButtonCell *bootROMsButton;
@property IBOutlet NSPopUpButton *rumbleModePopupButton;
@property IBOutlet NSPopUpButton *hotkey1PopupButton;
@property IBOutlet NSPopUpButton *hotkey2PopupButton;
@property IBOutlet NSSlider *temperatureSlider;
@property IBOutlet NSSlider *interferenceSlider;
@property IBOutlet NSPopUpButton *dmgPopupButton;
@property IBOutlet NSPopUpButton *sgbPopupButton;
@property IBOutlet NSPopUpButton *cgbPopupButton;
@property IBOutlet NSPopUpButton *agbPopupButton;
@property IBOutlet NSPopUpButton *preferredJoypadButton;
@property IBOutlet NSPopUpButton *playerListButton;
@property IBOutlet NSButton *autoUpdatesCheckbox;
@property IBOutlet NSSlider *volumeSlider;
@property IBOutlet NSButton *OSDCheckbox;
@property IBOutlet NSButton *screenshotFilterCheckbox;
@property IBOutlet GBPaletteEditorController *paletteEditorController;
@property IBOutlet NSWindow *paletteEditor;
@property IBOutlet NSButton *joystickMBC7Checkbox;
@property IBOutlet NSButton *mouseMBC7Checkbox;
@property IBOutlet NSWindow *joyconsSheet;
@end
