#import <Cocoa/Cocoa.h>
#import <JoyKit/JoyKit.h>
#import "GBPaletteEditorController.h"

@interface GBPreferencesWindow : NSWindow <NSTableViewDelegate, NSTableViewDataSource, JOYListener>
@property (nonatomic, strong) IBOutlet NSTableView *controlsTableView;
@property (nonatomic, strong) IBOutlet NSPopUpButton *graphicsFilterPopupButton;
@property (nonatomic, strong) IBOutlet NSButton *analogControlsCheckbox;
@property (nonatomic, strong) IBOutlet NSButton *aspectRatioCheckbox;
@property (nonatomic, strong) IBOutlet NSPopUpButton *highpassFilterPopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *colorCorrectionPopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *frameBlendingModePopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *colorPalettePopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *displayBorderPopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *rewindPopupButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *rtcPopupButton;
@property (nonatomic, strong) IBOutlet NSButton *configureJoypadButton;
@property (nonatomic, strong) IBOutlet NSButton *skipButton;
@property (nonatomic, strong) IBOutlet NSMenuItem *bootROMsFolderItem;
@property (nonatomic, strong) IBOutlet NSPopUpButtonCell *bootROMsButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *rumbleModePopupButton;
@property (nonatomic, weak) IBOutlet NSSlider *temperatureSlider;
@property (nonatomic, weak) IBOutlet NSSlider *interferenceSlider;
@property (nonatomic, weak) IBOutlet NSPopUpButton *dmgPopupButton;
@property (nonatomic, weak) IBOutlet NSPopUpButton *sgbPopupButton;
@property (nonatomic, weak) IBOutlet NSPopUpButton *cgbPopupButton;
@property (nonatomic, weak) IBOutlet NSPopUpButton *preferredJoypadButton;
@property (nonatomic, weak) IBOutlet NSPopUpButton *playerListButton;
@property (nonatomic, weak) IBOutlet NSButton *autoUpdatesCheckbox;
@property (weak) IBOutlet NSSlider *volumeSlider;
@property (weak) IBOutlet NSButton *OSDCheckbox;
@property (weak) IBOutlet NSButton *screenshotFilterCheckbox;
@property (weak) IBOutlet GBPaletteEditorController *paletteEditorController;
@property (strong) IBOutlet NSWindow *paletteEditor;
@property (weak) IBOutlet NSButton *joystickMBC7Checkbox;
@property (weak) IBOutlet NSButton *mouseMBC7Checkbox;
@end
