#import "GBViewController.h"
#import "GBHorizontalLayout.h"
#import "GBVerticalLayout.h"
#import "GBViewMetal.h"
#import "GBAudioClient.h"
#import "GBROMManager.h"
#import "GBLoadROMTableViewController.h"
#import "GBBackgroundView.h"
#import "GBHapticManager.h"
#import "GBMenuViewController.h"
#include <Core/gb.h>

@implementation GBViewController
{
    GB_gameboy_t _gb;
    GBView *_gbView;
    volatile bool _running;
    volatile bool _stopping;
    bool _romLoaded;
    GBHorizontalLayout *_horizontalLayout;
    GBVerticalLayout *_verticalLayout;
    GBBackgroundView *_backgroundView;
    NSCondition *_audioLock;
    GB_sample_t *_audioBuffer;
    size_t _audioBufferSize;
    size_t _audioBufferPosition;
    size_t _audioBufferNeeded;
    GBAudioClient *_audioClient;
}

static void loadBootROM(GB_gameboy_t *gb, GB_boot_rom_t type)
{
    GBViewController *self = (__bridge GBViewController *)GB_get_user_data(gb);
    [self loadBootROM:type];
}

static void vblank(GB_gameboy_t *gb, GB_vblank_type_t type)
{
    GBViewController *self = (__bridge GBViewController *)GB_get_user_data(gb);
    [self vblankWithType:type];
}

static void consoleLog(GB_gameboy_t *gb, const char *string, GB_log_attributes attributes)
{
    static NSString *buffer = @"";
    buffer = [buffer stringByAppendingString:@(string)];
    if ([buffer containsString:@"\n"]) {
        NSLog(@"%@", buffer);
        buffer = @"";
    }
}

static uint32_t rgbEncode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 0) | (g << 8) | (b << 16) | 0xFF000000;
}

static void audioCallback(GB_gameboy_t *gb, GB_sample_t *sample)
{
    GBViewController *self = (__bridge GBViewController *)GB_get_user_data(gb);
    [self gotNewSample:sample];
}

static void rumbleCallback(GB_gameboy_t *gb, double amp)
{
    GBViewController *self = (__bridge GBViewController *)GB_get_user_data(gb);
    [self rumbleChanged:amp];
}

- (void)initGameBoy
{
    GB_init(&_gb, GB_MODEL_CGB_E);
    GB_set_user_data(&_gb, (__bridge void *)(self));
    GB_set_boot_rom_load_callback(&_gb, (GB_boot_rom_load_callback_t)loadBootROM);
    GB_set_vblank_callback(&_gb, (GB_vblank_callback_t) vblank);
    GB_set_log_callback(&_gb, (GB_log_callback_t) consoleLog);
    GB_set_color_correction_mode(&_gb, (GB_color_correction_mode_t) [[NSUserDefaults standardUserDefaults] integerForKey:@"GBColorCorrection"]);
    GB_set_light_temperature(&_gb, [[NSUserDefaults standardUserDefaults] doubleForKey:@"GBLightTemperature"]);
    GB_set_interference_volume(&_gb, [[NSUserDefaults standardUserDefaults] doubleForKey:@"GBInterferenceVolume"]);
    GB_set_border_mode(&_gb, GB_BORDER_NEVER);
    [self updatePalette];
    GB_set_rgb_encode_callback(&_gb, rgbEncode);
    GB_set_highpass_filter_mode(&_gb, (GB_highpass_mode_t) [[NSUserDefaults standardUserDefaults] integerForKey:@"GBHighpassFilter"]);
    GB_set_rtc_mode(&_gb, [[NSUserDefaults standardUserDefaults] integerForKey:@"GBRTCMode"]);
    GB_apu_set_sample_callback(&_gb, audioCallback);
    GB_set_rumble_callback(&_gb, rumbleCallback);
    [self updateRumbleMode];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _window = [[UIWindow alloc] init];
    _window.rootViewController = self;
    [_window makeKeyAndVisible];
    
    _window.backgroundColor = [UIColor colorWithRed:174 / 255.0 green:176 / 255.0 blue:180 / 255.0 alpha:1.0];
    
    _horizontalLayout = [[GBHorizontalLayout alloc] init];
    _verticalLayout = [[GBVerticalLayout alloc] init];
    
    _backgroundView = [[GBBackgroundView alloc] init];
    [_window addSubview:_backgroundView];
    self.view = _backgroundView;
    
    
    [self initGameBoy];
    _gbView = _backgroundView.gbView;
    _gbView.gb = &_gb;
    [_gbView screenSizeChanged];
    
    [self willRotateToInterfaceOrientation:[UIApplication sharedApplication].statusBarOrientation
                                  duration:0];

    
    _audioLock = [[NSCondition alloc] init];
    
    [self loadROM];
    [[NSNotificationCenter defaultCenter] addObserverForName:@"GBROMChanged"
                                                      object:nil
                                                       queue:nil
                                                  usingBlock:^(NSNotification *note) {
        [self loadROM];
        [self start];
    }];
    return true;
}

- (void)loadROM
{
    [self stop];
    GBROMManager *romManager = [GBROMManager sharedManager];
    if (romManager.romFile) {
        // Todo: display errors and warnings
        _romLoaded = GB_load_rom(&_gb, romManager.romFile.fileSystemRepresentation) == 0;
        if (_romLoaded) {
            GB_reset(&_gb);
            GB_load_battery(&_gb, [GBROMManager sharedManager].batterySaveFile.fileSystemRepresentation);
            GB_load_state(&_gb, [GBROMManager sharedManager].autosaveStateFile.fileSystemRepresentation);
        }
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if (self.presentedViewController) return;
    [self start];
}

-(void)presentViewController:(UIViewController *)viewControllerToPresent animated:(BOOL)flag completion:(void (^)(void))completion
{
    [self stop];
    [super presentViewController:viewControllerToPresent
                        animated:flag
                      completion:completion];
}

- (void)reset
{
    [self stop];
    GB_reset(&_gb);
    [self start];
}

- (void)openLibrary
{
    [self presentViewController:[[GBLoadROMTableViewController alloc] init]
                       animated:true
                     completion:nil];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [self stop];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)orientation duration:(NSTimeInterval)duration
{
    GBLayout *layout = _horizontalLayout;
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) {
        layout = _verticalLayout;
    }
    _backgroundView.frame = [layout viewRectForOrientation:orientation];
    _backgroundView.layout = layout;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    if ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        return UIInterfaceOrientationMaskAll;
    }
    if (MAX([UIScreen mainScreen].bounds.size.height, [UIScreen mainScreen].bounds.size.width) <= 568) {
        return UIInterfaceOrientationMaskLandscape;
    }
    return UIInterfaceOrientationMaskAllButUpsideDown;
}

- (BOOL)prefersHomeIndicatorAutoHidden
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) {
        return false;
    }
    return true;
}

- (void)preRun
{
    GB_set_pixels_output(&_gb, _gbView.pixels);
    GB_set_sample_rate(&_gb, 96000);
    _audioClient = [[GBAudioClient alloc] initWithRendererBlock:^(UInt32 sampleRate, UInt32 nFrames, GB_sample_t *buffer) {
        [_audioLock lock];
        
        if (_audioBufferPosition < nFrames) {
            _audioBufferNeeded = nFrames;
            [_audioLock waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.125]];
        }
        
        if (_stopping) {
            memset(buffer, 0, nFrames * sizeof(*buffer));
            [_audioLock unlock];
            return;
        }
        
        if (_audioBufferPosition < nFrames) {
            // Not enough audio
            memset(buffer, 0, (nFrames - _audioBufferPosition) * sizeof(*buffer));
            memcpy(buffer, _audioBuffer, _audioBufferPosition * sizeof(*buffer));
            _audioBufferPosition = 0;
        }
        else if (_audioBufferPosition < nFrames + 4800) {
            memcpy(buffer, _audioBuffer, nFrames * sizeof(*buffer));
            memmove(_audioBuffer, _audioBuffer + nFrames, (_audioBufferPosition - nFrames) * sizeof(*buffer));
            _audioBufferPosition = _audioBufferPosition - nFrames;
        }
        else {
            memcpy(buffer, _audioBuffer + (_audioBufferPosition - nFrames), nFrames * sizeof(*buffer));
            _audioBufferPosition = 0;
        }
        [_audioLock unlock];
    } andSampleRate:96000];
    
    [_audioClient start];
}

- (void)run
{
    [self preRun];
    while (_running) {
        GB_run(&_gb);
    }
    [self postRun];
    _stopping = false;
}

- (void)postRun
{
    [_audioLock lock];
    memset(_audioBuffer, 0, (_audioBufferSize - _audioBufferPosition) * sizeof(*_audioBuffer));
    _audioBufferPosition = _audioBufferNeeded;
    [_audioLock signal];
    [_audioLock unlock];
    [_audioClient stop];
    _audioClient = nil;

    GB_save_battery(&_gb, [GBROMManager sharedManager].batterySaveFile.fileSystemRepresentation);
    GB_save_state(&_gb, [GBROMManager sharedManager].autosaveStateFile.fileSystemRepresentation);
}

- (void)start
{
    if (!_romLoaded) return;
    if (_running) return;
    _running = true;
    [[[NSThread alloc] initWithTarget:self selector:@selector(run) object:nil] start];
}

- (void)stop
{
    if (!_running) return;
    [_audioLock lock];
    _stopping = true;
    [_audioLock signal];
    [_audioLock unlock];
    _running = false;
    while (_stopping) {
        [_audioLock lock];
        [_audioLock signal];
        [_audioLock unlock];
    }
}
- (void)loadBootROM: (GB_boot_rom_t)type
{
    static NSString *const names[] = {
        [GB_BOOT_ROM_DMG_0] = @"dmg0_boot",
        [GB_BOOT_ROM_DMG] = @"dmg_boot",
        [GB_BOOT_ROM_MGB] = @"mgb_boot",
        [GB_BOOT_ROM_SGB] = @"sgb_boot",
        [GB_BOOT_ROM_SGB2] = @"sgb2_boot",
        [GB_BOOT_ROM_CGB_0] = @"cgb0_boot",
        [GB_BOOT_ROM_CGB] = @"cgb_boot",
        [GB_BOOT_ROM_AGB] = @"agb_boot",
    };
    GB_load_boot_rom(&_gb, [[[NSBundle mainBundle] pathForResource:names[type] ofType:@"bin"] UTF8String]);
}

- (void)vblankWithType:(GB_vblank_type_t)type
{
    if (type != GB_VBLANK_TYPE_REPEAT) {
        [_gbView flip];
        GB_set_pixels_output(&_gb, _gbView.pixels);
    }
}

- (void)gotNewSample:(GB_sample_t *)sample
{
    [_audioLock lock];
    if (_audioClient.isPlaying) {
        if (_audioBufferPosition == _audioBufferSize) {
            if (_audioBufferSize >= 0x4000) {
                _audioBufferPosition = 0;
                [_audioLock unlock];
                return;
            }
            
            if (_audioBufferSize == 0) {
                _audioBufferSize = 512;
            }
            else {
                _audioBufferSize += _audioBufferSize >> 2;
            }
            _audioBuffer = realloc(_audioBuffer, sizeof(*sample) * _audioBufferSize);
        }
        _audioBuffer[_audioBufferPosition++] = *sample;
    }
    if (_audioBufferPosition == _audioBufferNeeded) {
        [_audioLock signal];
        _audioBufferNeeded = 0;
    }
    [_audioLock unlock];
}

- (void)rumbleChanged:(double)amp
{
    [[GBHapticManager sharedManager] setRumbleStrength:amp];
}

- (void)updateRumbleMode
{
    GB_set_rumble_mode(&_gb, [[NSUserDefaults standardUserDefaults] integerForKey:@"GBRumbleMode"]);
}

- (const GB_palette_t *)userPalette
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    switch ([defaults integerForKey:@"GBColorPalette"]) {
        case 1: return &GB_PALETTE_DMG;
        case 2: return &GB_PALETTE_MGB;
        case 3: return &GB_PALETTE_GBL;
        default: return &GB_PALETTE_GREY;
        case -1: {
            static GB_palette_t customPalette;
            NSArray *colors = [defaults dictionaryForKey:@"GBThemes"][[defaults stringForKey:@"GBCurrentTheme"]][@"Colors"];
            if (colors.count == 5) {
                unsigned i = 0;
                for (NSNumber *color in colors) {
                    uint32_t c = [color unsignedIntValue];
                    customPalette.colors[i++] = (struct GB_color_s) {c, c >> 8, c >> 16};
                }
            }
            return &customPalette;
        }
    }
}

- (void)updatePalette
{
    GB_set_palette(&_gb, [self userPalette]);
}
@end
