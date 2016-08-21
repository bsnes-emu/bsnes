#include <CoreAudio/CoreAudio.h>
#include "GBAudioClient.h"
#include "Document.h"
#include "AppDelegate.h"
#include "gb.h"
#include "debugger.h"
#include "memory.h"
#include "HexFiend/HexFiend.h"
#include "GBMemoryByteArray.h"

/* Todo: The general Objective-C coding style conflicts with SameBoy's. This file needs a cleanup. */

@interface Document ()
{
    /* NSTextViews freeze the entire app if they're modified too often and too quickly.
     We use this bool to tune down the write speed. Let me know if there's a more
     reasonable alternative to this. */
    unsigned long pendingLogLines;
    bool tooMuchLogs;
    bool fullScreen;
    bool in_sync_input;
    HFController *hex_controller;

    NSString *lastConsoleInput;
    HFLineCountingRepresenter *lineRep;
}

@property GBAudioClient *audioClient;
- (void) vblank;
- (void) log: (const char *) log withAttributes: (GB_log_attributes) attributes;
- (const char *) getDebuggerInput;
- (const char *) getAsyncDebuggerInput;
@end

static void vblank(GB_gameboy_t *gb)
{
    Document *self = (__bridge Document *)(gb->user_data);
    [self vblank];
}

static void consoleLog(GB_gameboy_t *gb, const char *string, GB_log_attributes attributes)
{
    Document *self = (__bridge Document *)(gb->user_data);
    [self log:string withAttributes: attributes];
}

static char *consoleInput(GB_gameboy_t *gb)
{
    Document *self = (__bridge Document *)(gb->user_data);
    return strdup([self getDebuggerInput]);
}

static char *asyncConsoleInput(GB_gameboy_t *gb)
{
    Document *self = (__bridge Document *)(gb->user_data);
    const char *ret = [self getAsyncDebuggerInput];
    return ret? strdup(ret) : NULL;
}

static uint32_t rgbEncode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 0) | (g << 8) | (b << 16);
}

@implementation Document
{
    GB_gameboy_t gb;
    volatile bool running;
    volatile bool stopping;
    NSConditionLock *has_debugger_input;
    NSMutableArray *debugger_input_queue;
    volatile bool is_inited;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        has_debugger_input = [[NSConditionLock alloc] initWithCondition:0];
        debugger_input_queue = [[NSMutableArray alloc] init];
        if ([[NSUserDefaults standardUserDefaults] boolForKey:@"EmulateDMG"]) {
            [self initDMG];
        }
        else {
            [self initCGB];
        }
    }
    return self;
}

- (void) initDMG
{
    GB_init(&gb);
    GB_load_boot_rom(&gb, [[[NSBundle mainBundle] pathForResource:@"dmg_boot" ofType:@"bin"] UTF8String]);
    GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    GB_set_log_callback(&gb, (GB_log_callback_t) consoleLog);
    GB_set_input_callback(&gb, (GB_input_callback_t) consoleInput);
    GB_set_async_input_callback(&gb, (GB_input_callback_t) asyncConsoleInput);
    GB_set_rgb_encode_callback(&gb, rgbEncode);
    gb.user_data = (__bridge void *)(self);
}

- (void) initCGB
{
    GB_init_cgb(&gb);
    GB_load_boot_rom(&gb, [[[NSBundle mainBundle] pathForResource:@"cgb_boot" ofType:@"bin"] UTF8String]);
    GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    GB_set_log_callback(&gb, (GB_log_callback_t) consoleLog);
    GB_set_input_callback(&gb, (GB_input_callback_t) consoleInput);
    GB_set_async_input_callback(&gb, (GB_input_callback_t) asyncConsoleInput);
    GB_set_rgb_encode_callback(&gb, rgbEncode);
    gb.user_data = (__bridge void *)(self);
}

- (void) vblank
{
    self.view.mouseHidingEnabled = (self.mainWindow.styleMask & NSFullScreenWindowMask) != 0;
    [self.view flip];
    GB_set_pixels_output(&gb, self.view.pixels);
}

- (void) run
{
    running = true;
    GB_set_pixels_output(&gb, self.view.pixels);
    self.view.gb = &gb;
    GB_set_sample_rate(&gb, 96000);
    self.audioClient = [[GBAudioClient alloc] initWithRendererBlock:^(UInt32 sampleRate, UInt32 nFrames, GB_sample_t *buffer) {
        GB_apu_copy_buffer(&gb, buffer, nFrames);
    } andSampleRate:96000];
    self.view.mouseHidingEnabled = (self.mainWindow.styleMask & NSFullScreenWindowMask) != 0;
    if (![[NSUserDefaults standardUserDefaults] boolForKey:@"Mute"]) {
        [self.audioClient start];
    }
    NSTimer *hex_timer = [NSTimer timerWithTimeInterval:0.25 target:self selector:@selector(reloadMemoryView) userInfo:nil repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:hex_timer forMode:NSDefaultRunLoopMode];
    while (running) {
        GB_run(&gb);
    }
    [hex_timer invalidate];
    [self.audioClient stop];
    self.audioClient = nil;
    self.view.mouseHidingEnabled = NO;
    GB_save_battery(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"sav"] UTF8String]);
    stopping = false;
}

- (void) start
{
    if (running) return;
    [[[NSThread alloc] initWithTarget:self selector:@selector(run) object:nil] start];
}

- (void) stop
{
    if (!running) return;
    if (gb.debug_stopped) {
        gb.debug_stopped = false;
        [self consoleInput:nil];
    }
    stopping = true;
    running = false;
    while (stopping);
}

- (IBAction)reset:(id)sender
{
    bool was_cgb = gb.is_cgb;
    [self stop];
    is_inited = false;
    GB_free(&gb);
    if (([sender tag] == 0 && was_cgb) || [sender tag] == 2) {
        [self initCGB];
    }
    else {
        [self initDMG];
    }
    if ([sender tag] != 0) {
        /* User explictly selected a model, save the preference */
        [[NSUserDefaults standardUserDefaults] setBool:!gb.is_cgb forKey:@"EmulateDMG"];
    }
    [self readFromFile:self.fileName ofType:@"gb"];
    [self start];

    if (hex_controller) {
        /* Verify bank sanity, especially when switching models. */
        [(GBMemoryByteArray *)(hex_controller.byteArray) setSelectedBank:0];
        [self hexUpdateBank:self.memoryBankInput];
    }
}

- (IBAction)togglePause:(id)sender
{
    if (running) {
        [self stop];
    }
    else {
        [self start];
    }
}

- (void)dealloc
{
    GB_free(&gb);
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController {
    [super windowControllerDidLoadNib:aController];
    self.consoleOutput.textContainerInset = NSMakeSize(4, 4);
    [self.view becomeFirstResponder];
    self.view.shouldBlendFrameWithPrevious = ![[NSUserDefaults standardUserDefaults] boolForKey:@"DisableFrameBlending"];
    CGRect window_frame = self.mainWindow.frame;
    window_frame.size.width  = MAX([[NSUserDefaults standardUserDefaults] integerForKey:@"LastWindowWidth"],
                                  window_frame.size.width);
    window_frame.size.height = MAX([[NSUserDefaults standardUserDefaults] integerForKey:@"LastWindowHeight"],
                                   window_frame.size.height);
    [self.mainWindow setFrame:window_frame display:YES];
    [self start];

}

- (void) initMemoryView
{
    hex_controller = [[HFController alloc] init];
    [hex_controller setBytesPerColumn:1];
    [hex_controller setFont:[NSFont userFixedPitchFontOfSize:12]];
    [hex_controller setEditMode:HFOverwriteMode];
    
    [hex_controller setByteArray:[[GBMemoryByteArray alloc] initWithDocument:self]];

    /* Here we're going to make three representers - one for the hex, one for the ASCII, and one for the scrollbar.  To lay these all out properly, we'll use a fourth HFLayoutRepresenter. */
    HFLayoutRepresenter *layoutRep = [[HFLayoutRepresenter alloc] init];
    HFHexTextRepresenter *hexRep = [[HFHexTextRepresenter alloc] init];
    HFStringEncodingTextRepresenter *asciiRep = [[HFStringEncodingTextRepresenter alloc] init];
    HFVerticalScrollerRepresenter *scrollRep = [[HFVerticalScrollerRepresenter alloc] init];
    lineRep = [[HFLineCountingRepresenter alloc] init];
    HFStatusBarRepresenter *statusRep = [[HFStatusBarRepresenter alloc] init];

    lineRep.lineNumberFormat = HFLineNumberFormatHexadecimal;

    /* Add all our reps to the controller. */
    [hex_controller addRepresenter:layoutRep];
    [hex_controller addRepresenter:hexRep];
    [hex_controller addRepresenter:asciiRep];
    [hex_controller addRepresenter:scrollRep];
    [hex_controller addRepresenter:lineRep];
    [hex_controller addRepresenter:statusRep];

    /* Tell the layout rep which reps it should lay out. */
    [layoutRep addRepresenter:hexRep];
    [layoutRep addRepresenter:scrollRep];
    [layoutRep addRepresenter:asciiRep];
    [layoutRep addRepresenter:lineRep];
    [layoutRep addRepresenter:statusRep];


    [(NSView *)[hexRep view] setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    /* Grab the layout rep's view and stick it into our container. */
    NSView *layoutView = [layoutRep view];
    NSRect layoutViewFrame = self.memoryView.frame;
    [layoutView setFrame:layoutViewFrame];
    [layoutView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable | NSViewMaxYMargin];
    [self.memoryView addSubview:layoutView];

    self.memoryBankItem.enabled = false;
}

+ (BOOL)autosavesInPlace {
    return YES;
}

- (NSString *)windowNibName {
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"Document";
}

- (BOOL)readFromFile:(NSString *)fileName ofType:(NSString *)type
{
    if (is_inited++) {
        return YES;
    }
    GB_load_rom(&gb, [fileName UTF8String]);
    GB_load_battery(&gb, [[[fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"sav"] UTF8String]);
    GB_debugger_load_symbol_file(&gb, [[[fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"sym"] UTF8String]);

    return YES;
}

- (void)close
{
    [[NSUserDefaults standardUserDefaults] setInteger:self.mainWindow.frame.size.width forKey:@"LastWindowWidth"];
    [[NSUserDefaults standardUserDefaults] setInteger:self.mainWindow.frame.size.height forKey:@"LastWindowHeight"];
    [self stop];
    [self.consoleWindow close];
    [super close];
}

- (IBAction) interrupt:(id)sender
{
    [self log:"^C\n"];
    gb.debug_stopped = true;
    if (!running) {
        [self start];
    }
    [self.consoleInput becomeFirstResponder];
}

- (IBAction)mute:(id)sender
{
    if (self.audioClient.isPlaying) {
        [self.audioClient stop];
    }
    else {
        [self.audioClient start];
    }
    [[NSUserDefaults standardUserDefaults] setBool:!self.audioClient.isPlaying forKey:@"Mute"];
}

- (IBAction)toggleBlend:(id)sender
{
    self.view.shouldBlendFrameWithPrevious ^= YES;
    [[NSUserDefaults standardUserDefaults] setBool:!self.view.shouldBlendFrameWithPrevious forKey:@"DisableFrameBlending"];
}

- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)anItem
{
    if([anItem action] == @selector(mute:)) {
        [(NSMenuItem*)anItem setState:!self.audioClient.isPlaying];
    }
    else if ([anItem action] == @selector(togglePause:)) {
        [(NSMenuItem*)anItem setState:(!running) || (gb.debug_stopped)];
        return !gb.debug_stopped;
    }
    else if ([anItem action] == @selector(reset:) && anItem.tag != 0) {
        [(NSMenuItem*)anItem setState:(anItem.tag == 1 && !gb.is_cgb) || (anItem.tag == 2 && gb.is_cgb)];
    }
    else if ([anItem action] == @selector(toggleBlend:)) {
        [(NSMenuItem*)anItem setState:self.view.shouldBlendFrameWithPrevious];
    }
    else if ([anItem action] == @selector(interrupt:)) {
        if (![[NSUserDefaults standardUserDefaults] boolForKey:@"DeveloperMode"]) {
            return false;
        }
    }
    return [super validateUserInterfaceItem:anItem];
}


- (void) windowWillEnterFullScreen:(NSNotification *)notification
{
    fullScreen = true;
    self.view.mouseHidingEnabled = running;
}

- (void) windowWillExitFullScreen:(NSNotification *)notification
{
    fullScreen = false;
    self.view.mouseHidingEnabled = NO;
}

- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)newFrame
{
    if (fullScreen) {
        return newFrame;
    }
    NSRect rect = window.contentView.frame;

    int titlebarSize = window.contentView.superview.frame.size.height - rect.size.height;
    int step = 160 / [[window screen] backingScaleFactor];

    rect.size.width = floor(rect.size.width / step) * step + step;
    rect.size.height = rect.size.width / 10 * 9 + titlebarSize;

    if (rect.size.width > newFrame.size.width) {
        rect.size.width = 160;
        rect.size.height = 144 + titlebarSize;
    }
    else if (rect.size.height > newFrame.size.height) {
        rect.size.width = 160;
        rect.size.height = 144 + titlebarSize;
    }

    rect.origin = window.frame.origin;
    rect.origin.y -= rect.size.height - window.frame.size.height;

    return rect;
}

- (void) log: (const char *) string withAttributes: (GB_log_attributes) attributes
{
    if (pendingLogLines > 128) {
        /* The ROM causes so many errors in such a short time, and we can't handle it. */
        tooMuchLogs = true;
        return;
    }
    pendingLogLines++;

    /* Make sure mouse is not hidden while debugging */
    self.view.mouseHidingEnabled = NO;

    NSString *nsstring = @(string); // For ref-counting
    dispatch_async(dispatch_get_main_queue(), ^{
        [hex_controller reloadData];

        NSFont *font = [NSFont userFixedPitchFontOfSize:12];
        NSUnderlineStyle underline = NSUnderlineStyleNone;
        if (attributes & GB_LOG_BOLD) {
            font = [[NSFontManager sharedFontManager] convertFont:font toHaveTrait:NSBoldFontMask];
        }

        if (attributes &  GB_LOG_UNDERLINE_MASK) {
            underline = (attributes &  GB_LOG_UNDERLINE_MASK) == GB_LOG_DASHED_UNDERLINE? NSUnderlinePatternDot | NSUnderlineStyleSingle : NSUnderlineStyleSingle;
        }

        NSMutableParagraphStyle *paragraph_style = [[NSMutableParagraphStyle alloc] init];
        [paragraph_style setLineSpacing:2];
        NSAttributedString *attributed =
            [[NSAttributedString alloc] initWithString:nsstring
                                            attributes:@{NSFontAttributeName: font,
                                                         NSForegroundColorAttributeName: [NSColor whiteColor],
                                                         NSUnderlineStyleAttributeName: @(underline),
                                                         NSParagraphStyleAttributeName: paragraph_style}];
        [self.consoleOutput.textStorage appendAttributedString:attributed];
        if (pendingLogLines == 1) {
            if (tooMuchLogs) {
                tooMuchLogs = false;
                [self log:"[...]\n"];
            }
            [self.consoleOutput scrollToEndOfDocument:nil];
            if ([[NSUserDefaults standardUserDefaults] boolForKey:@"DeveloperMode"]) {
                [self.consoleWindow orderBack:nil];
            }
        }
        pendingLogLines--;
    });
}

- (IBAction)showConsoleWindow:(id)sender
{
    [self.consoleWindow orderBack:nil];
}

- (IBAction)consoleInput:(NSTextField *)sender {
    NSString *line = [sender stringValue];
    if ([line isEqualToString:@""] && lastConsoleInput) {
        line = lastConsoleInput;
    }
    else if (line) {
        lastConsoleInput = line;
    }
    else {
        line = @"";
    }

    if (!in_sync_input) {
        [self log:">"];
    }
    [self log:[line UTF8String]];
    [self log:"\n"];
    [has_debugger_input lock];
    [debugger_input_queue addObject:line];
    [has_debugger_input unlockWithCondition:1];

    [sender setStringValue:@""];
}

- (const char *) getDebuggerInput
{
    [self log:">"];
    in_sync_input = true;
    [has_debugger_input lockWhenCondition:1];
    NSString *input = [debugger_input_queue firstObject];
    [debugger_input_queue removeObjectAtIndex:0];
    [has_debugger_input unlockWithCondition:[debugger_input_queue count] != 0];
    in_sync_input = false;
    return [input UTF8String];
}

- (const char *) getAsyncDebuggerInput
{
    [has_debugger_input lock];
    NSString *input = [debugger_input_queue firstObject];
    if (input) {
        [debugger_input_queue removeObjectAtIndex:0];
    }
    [has_debugger_input unlockWithCondition:[debugger_input_queue count] != 0];
    return [input UTF8String];
}

- (IBAction)saveState:(id)sender
{
    bool was_running = running;
    if (!gb.debug_stopped) {
        [self stop];
    }
    GB_save_state(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:[NSString stringWithFormat:@"s%ld", (long)[sender tag] ]] UTF8String]);
    if (was_running) {
        [self start];
    }
}

- (IBAction)loadState:(id)sender
{
    bool was_running = running;
    if (!gb.debug_stopped) {
        [self stop];
    }
    GB_load_state(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:[NSString stringWithFormat:@"s%ld", (long)[sender tag] ]] UTF8String]);
    if (was_running) {
        [self start];
    }
}

- (IBAction)clearConsole:(id)sender
{
    [self.consoleOutput setString:@""];
}

- (void)log:(const char *)log
{
    [self log:log withAttributes:0];
}

- (uint8_t) readMemory:(uint16_t)addr
{
    while (!is_inited);
    return GB_read_memory(&gb, addr);
}

- (void) writeMemory:(uint16_t)addr value:(uint8_t)value
{
    while (!is_inited);
    GB_write_memory(&gb, addr, value);
}

- (void) performAtomicBlock: (void (^)())block
{
    while (!is_inited);
    bool was_running = running && !gb.debug_stopped;
    if (was_running) {
        [self stop];
    }
    block();
    if (was_running) {
        [self start];
    }
}

- (void) reloadMemoryView
{
    if (self.memoryWindow.isVisible) {
        [hex_controller reloadData];
    }
}

- (IBAction) showMemory:(id)sender
{
    if (!hex_controller) {
        [self initMemoryView];
    }
    [self.memoryWindow makeKeyAndOrderFront:sender];
}

- (IBAction)hexGoTo:(id)sender
{
    [self performAtomicBlock:^{
        uint16_t addr;
        if (GB_debugger_evaluate(&gb, [[sender stringValue] UTF8String], &addr, NULL)) {
            NSBeep();
            return;
        }
        addr -= lineRep.valueOffset;
        if (addr >= hex_controller.byteArray.length) {
            NSBeep();
            return;
        }
        [hex_controller setSelectedContentsRanges:@[[HFRangeWrapper withRange:HFRangeMake(addr, 0)]]];
        [hex_controller _ensureVisibilityOfLocation:addr];
        [self.memoryWindow makeFirstResponder:self.memoryView.subviews[0].subviews[0]];
    }];
}

- (IBAction)hexUpdateBank:(NSControl *)sender
{
    [self performAtomicBlock:^{
        uint16_t addr, bank;
        if (GB_debugger_evaluate(&gb, [[sender stringValue] UTF8String], &addr, &bank)) {
            NSBeep();
            return;
        }

        if (bank == (uint16_t) -1) {
            bank = addr;
        }

        uint16_t n_banks = 1;
        switch ([(GBMemoryByteArray *)(hex_controller.byteArray) mode]) {
            case GBMemoryROM:
                n_banks = gb.rom_size / 0x4000;
                break;
            case GBMemoryVRAM:
                n_banks = gb.is_cgb ? 2 : 1;
                break;
            case GBMemoryExternalRAM:
                n_banks = gb.mbc_ram_size / 0x2000;
                break;
            case GBMemoryRAM:
                n_banks = gb.is_cgb ? 8 : 1;
                break;
            case GBMemoryEntireSpace:
                break;
        }

        bank %= n_banks;

        [sender setStringValue:[NSString stringWithFormat:@"$%x", bank]];
        [(GBMemoryByteArray *)(hex_controller.byteArray) setSelectedBank:bank];
        [hex_controller reloadData];
    }];
}

- (IBAction)hexUpdateSpace:(NSPopUpButtonCell *)sender
{
    self.memoryBankItem.enabled = [sender indexOfSelectedItem] != GBMemoryEntireSpace;
    GBMemoryByteArray *byteArray = (GBMemoryByteArray *)(hex_controller.byteArray);
    [byteArray setMode:(GB_memory_mode_t)[sender indexOfSelectedItem]];
    switch ((GB_memory_mode_t)[sender indexOfSelectedItem]) {
        case GBMemoryEntireSpace:
        case GBMemoryROM:
            lineRep.valueOffset = 0;
            byteArray.selectedBank = gb.mbc_rom_bank;
            break;
        case GBMemoryVRAM:
            lineRep.valueOffset = 0x8000;
            byteArray.selectedBank = gb.cgb_vram_bank;
            break;
        case GBMemoryExternalRAM:
            lineRep.valueOffset = 0xA000;
            byteArray.selectedBank = gb.mbc_ram_bank;
            break;
        case GBMemoryRAM:
            lineRep.valueOffset = 0xC000;
            byteArray.selectedBank = gb.cgb_ram_bank;
            break;
    }
    [self.memoryBankInput setStringValue:[NSString stringWithFormat:@"$%x", byteArray.selectedBank]];
    [hex_controller reloadData];
    [self.memoryView setNeedsDisplay:YES];
}

- (GB_gameboy_t *) gameboy
{
    return &gb;
}

@end