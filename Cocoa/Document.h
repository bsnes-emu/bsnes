#import <Cocoa/Cocoa.h>
#import "GBView.h"
#import "GBImageView.h"
#import "GBSplitView.h"
#import "GBVisualizerView.h"
#import "GBOSDView.h"
#import "GBDebuggerButton.h"

@class GBCheatWindowController;
@class GBPaletteView;
@class GBObjectView;

@interface Document : NSDocument <NSWindowDelegate, GBImageViewDelegate, NSSplitViewDelegate>
@property (nonatomic, readonly) GB_gameboy_t *gb;
@property (nonatomic, strong) IBOutlet GBView *view;
@property (nonatomic, strong) IBOutlet NSTextView *consoleOutput;
@property (nonatomic, strong) IBOutlet NSPanel *consoleWindow;
@property (nonatomic, strong) IBOutlet NSTextField *consoleInput;
@property (nonatomic, strong) IBOutlet NSWindow *mainWindow;
@property (nonatomic, strong) IBOutlet NSView *memoryView;
@property (nonatomic, strong) IBOutlet NSPanel *memoryWindow;
@property (nonatomic, readonly) GB_gameboy_t *gameboy;
@property (nonatomic, strong) IBOutlet NSTextField *memoryBankInput;
@property (nonatomic, strong) IBOutlet NSToolbarItem *memoryBankItem;
@property (nonatomic, strong) IBOutlet GBImageView *tilesetImageView;
@property (nonatomic, strong) IBOutlet NSPopUpButton *tilesetPaletteButton;
@property (nonatomic, strong) IBOutlet GBImageView *tilemapImageView;
@property (nonatomic, strong) IBOutlet NSPopUpButton *tilemapPaletteButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *tilemapMapButton;
@property (nonatomic, strong) IBOutlet NSPopUpButton *TilemapSetButton;
@property (nonatomic, strong) IBOutlet NSButton *gridButton;
@property (nonatomic, strong) IBOutlet NSTabView *vramTabView;
@property (nonatomic, strong) IBOutlet NSPanel *vramWindow;
@property (nonatomic, strong) IBOutlet NSTextField *vramStatusLabel;
@property (nonatomic, strong) IBOutlet GBPaletteView *paletteView;
@property (nonatomic, strong) IBOutlet GBObjectView *objectView;
@property (nonatomic, strong) IBOutlet NSPanel *printerFeedWindow;
@property (nonatomic, strong) IBOutlet NSImageView *feedImageView;
@property (nonatomic, strong) IBOutlet NSTextView *debuggerSideViewInput;
@property (nonatomic, strong) IBOutlet NSTextView *debuggerSideView;
@property (nonatomic, strong) IBOutlet GBSplitView *debuggerSplitView;
@property (nonatomic, strong) IBOutlet NSBox *debuggerVerticalLine;
@property (nonatomic, strong) IBOutlet NSPanel *cheatsWindow;
@property (nonatomic, strong) IBOutlet GBCheatWindowController *cheatWindowController;
@property (nonatomic, readonly) Document *partner;
@property (nonatomic, readonly) bool isSlave;
@property (strong) IBOutlet NSView *gbsPlayerView;
@property (strong) IBOutlet NSTextField *gbsTitle;
@property (strong) IBOutlet NSTextField *gbsAuthor;
@property (strong) IBOutlet NSTextField *gbsCopyright;
@property (strong) IBOutlet NSPopUpButton *gbsTracks;
@property (strong) IBOutlet NSButton *gbsPlayPauseButton;
@property (strong) IBOutlet NSButton *gbsRewindButton;
@property (strong) IBOutlet NSSegmentedControl *gbsNextPrevButton;
@property (strong) IBOutlet GBVisualizerView *gbsVisualizer;
@property (strong) IBOutlet GBOSDView *osdView;
@property (readonly) GB_oam_info_t *oamInfo;
@property uint8_t oamCount;
@property uint8_t oamHeight;
@property (strong) IBOutlet NSView *audioRecordingAccessoryView;
@property (strong) IBOutlet NSPopUpButton *audioFormatButton;
@property (strong) IBOutlet NSVisualEffectView *debuggerSidebarEffectView API_AVAILABLE(macos(10.10));

@property (strong) IBOutlet GBDebuggerButton *debuggerContinueButton;
@property (strong) IBOutlet GBDebuggerButton *debuggerNextButton;
@property (strong) IBOutlet GBDebuggerButton *debuggerStepButton;
@property (strong) IBOutlet GBDebuggerButton *debuggerFinishButton;


+ (NSImage *) imageFromData:(NSData *)data width:(NSUInteger) width height:(NSUInteger) height scale:(double) scale;
-(uint8_t) readMemory:(uint16_t) addr;
-(void) writeMemory:(uint16_t) addr value:(uint8_t)value;
-(void) performAtomicBlock: (void (^)())block;
-(void) connectLinkCable:(NSMenuItem *)sender;
-(int)loadStateFile:(const char *)path noErrorOnNotFound:(bool)noErrorOnFileNotFound;
@end

