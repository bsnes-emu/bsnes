#include <CoreAudio/CoreAudio.h>
#include "AudioClient.h"
#import "Document.h"
#include "gb.h"

@interface Document ()
{
    /* NSTextViews freeze the entire app if they're modified too often and too quickly.
     We use this bool to tune down the write speed. Let me know if there's a more
     reasonable alternative to this. */
    unsigned long pendingLogLines;
    bool tooMuchLogs;
    
    NSString *lastConsoleInput;
}

@property AudioClient *audioClient;
- (void) vblank;
- (void) log: (const char *) log withAttributes: (gb_log_attributes) attributes;
- (const char *) getDebuggerInput;
@end

static void vblank(GB_gameboy_t *gb)
{
    Document *self = (__bridge Document *)(gb->user_data);
    [self vblank];
}

static void consoleLog(GB_gameboy_t *gb, const char *string, gb_log_attributes attributes)
{
    Document *self = (__bridge Document *)(gb->user_data);
    [self log:string withAttributes: attributes];
}

static char *consoleInput(GB_gameboy_t *gb)
{
    Document *self = (__bridge Document *)(gb->user_data);
    return strdup([self getDebuggerInput]);
}

static uint32_t rgbEncode(GB_gameboy_t *gb, unsigned char r, unsigned char g, unsigned char b)
{
    return (r << 24) | (g << 16) | (b << 8);
}

@implementation Document
{
    GB_gameboy_t gb;
    volatile bool running;
    volatile bool stopping;
    NSConditionLock *has_debugger_input;
    NSMutableArray *debugger_input_queue;
    bool is_inited;
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
    gb_init(&gb);
    gb_load_bios(&gb, [[[NSBundle mainBundle] pathForResource:@"dmg_boot" ofType:@"bin"] UTF8String]);
    gb_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    gb_set_log_callback(&gb, (GB_log_callback_t) consoleLog);
    gb_set_input_callback(&gb, (GB_input_callback_t) consoleInput);
    gb_set_rgb_encode_callback(&gb, rgbEncode);
    gb.user_data = (__bridge void *)(self);
}

- (void) initCGB
{
    gb_init_cgb(&gb);
    gb_load_bios(&gb, [[[NSBundle mainBundle] pathForResource:@"cgb_boot" ofType:@"bin"] UTF8String]);
    gb_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    gb_set_log_callback(&gb, (GB_log_callback_t) consoleLog);
    gb_set_input_callback(&gb, (GB_input_callback_t) consoleInput);
    gb_set_rgb_encode_callback(&gb, rgbEncode);
    gb.user_data = (__bridge void *)(self);
}

- (void) vblank
{
    [self.view flip];
    gb_set_pixels_output(&gb, self.view.pixels);
}

- (void) run
{
    running = true;
    gb_set_pixels_output(&gb, self.view.pixels);
    self.view.gb = &gb;
    gb_set_sample_rate(&gb, 96000);
    self.audioClient = [[AudioClient alloc] initWithRendererBlock:^(UInt32 sampleRate, UInt32 nFrames, SInt16 *buffer) {
        //apu_render(&gb, sampleRate, nFrames, buffer);
        apu_copy_buffer(&gb, buffer, nFrames);
    } andSampleRate:96000];
    [self.audioClient start];
    while (running) {
        gb_run(&gb);
    }
    [self.audioClient stop];
    gb_save_battery(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"sav"] UTF8String]);
    stopping = false;
}

- (void) start
{
    if (running) return;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self run];
    });
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
    gb_free(&gb);
    is_inited = false;
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
    gb_free(&gb);
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController {
    [super windowControllerDidLoadNib:aController];
    self.consoleOutput.textContainerInset = NSMakeSize(4, 4);
    [self.view becomeFirstResponder];
    self.view.shouldBlendFrameWithPrevious = ![[NSUserDefaults standardUserDefaults] boolForKey:@"DisableFrameBlending"];
    [self start];

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
    gb_load_rom(&gb, [fileName UTF8String]);
    gb_load_battery(&gb, [[[fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"sav"] UTF8String]);
    return YES;
}

- (void)close
{
    [self stop];
    [self.consoleWindow close];
    [super close];
}

- (IBAction) interrupt:(id)sender
{
    [self log:"^C\n"];
    gb.debug_stopped = true;
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
    if ([anItem action] == @selector(togglePause:)) {
        [(NSMenuItem*)anItem setState:!running];
    }
    if ([anItem action] == @selector(reset:) && anItem.tag != 0) {
        [(NSMenuItem*)anItem setState:(anItem.tag == 1 && !gb.is_cgb) || (anItem.tag == 2 && gb.is_cgb)];
    }
    if([anItem action] == @selector(toggleBlend:)) {
        [(NSMenuItem*)anItem setState:self.view.shouldBlendFrameWithPrevious];
    }
    return [super validateUserInterfaceItem:anItem];
}


- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)newFrame
{
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

- (void) log: (const char *) string withAttributes: (gb_log_attributes) attributes
{
    if (pendingLogLines > 128) {
        /* The ROM causes so many errors in such a short time, and we can't handle it. */
        tooMuchLogs = true;
        return;
    }
    pendingLogLines++;
    NSString *nsstring = @(string); // For ref-counting
    dispatch_async(dispatch_get_main_queue(), ^{
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
            [self.consoleWindow orderBack:nil];
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
    [has_debugger_input lockWhenCondition:1];
    NSString *input = [debugger_input_queue firstObject];
    [debugger_input_queue removeObjectAtIndex:0];
    [has_debugger_input unlockWithCondition:[debugger_input_queue count] != 0];
    return [input UTF8String];
}

- (IBAction)saveState:(id)sender
{
    bool was_running = running;
    if (!gb.debug_stopped) {
        [self stop];
    }
    gb_save_state(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:[NSString stringWithFormat:@"s%ld", (long)[sender tag] ]] UTF8String]);
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
    gb_load_state(&gb, [[[self.fileName stringByDeletingPathExtension] stringByAppendingPathExtension:[NSString stringWithFormat:@"s%ld", (long)[sender tag] ]] UTF8String]);
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

@end
