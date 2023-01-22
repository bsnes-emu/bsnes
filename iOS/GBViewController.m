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
#import "GBOptionViewController.h"
#import "GBAboutController.h"
#import "GBSettingsViewController.h"
#import "GBStatesViewController.h"
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
    NSMutableSet *_defaultsObservers;
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
    GB_gameboy_t *gb = &_gb;
    GB_init(gb, [[NSUserDefaults standardUserDefaults] integerForKey:@"GBCGBModel"]);
    GB_set_user_data(gb, (__bridge void *)(self));
    GB_set_boot_rom_load_callback(gb, (GB_boot_rom_load_callback_t)loadBootROM);
    GB_set_vblank_callback(gb, (GB_vblank_callback_t) vblank);
    GB_set_log_callback(gb, (GB_log_callback_t) consoleLog);
    [self addDefaultObserver:^(id newValue) {
        GB_set_color_correction_mode(gb, (GB_color_correction_mode_t)[newValue integerValue]);
    } forKey:@"GBColorCorrection"];
    [self addDefaultObserver:^(id newValue) {
        GB_set_light_temperature(gb, [newValue doubleValue]);
    } forKey:@"GBLightTemperature"];
    [self addDefaultObserver:^(id newValue) {
        GB_set_interference_volume(gb, [newValue doubleValue]);
    } forKey:@"GBInterferenceVolume"];
    GB_set_border_mode(gb, GB_BORDER_NEVER);
    __weak typeof(self) weakSelf = self;
    [self addDefaultObserver:^(id newValue) {
        [weakSelf updatePalette];
    } forKey:@"GBCurrentTheme"];
    GB_set_rgb_encode_callback(gb, rgbEncode);
    [self addDefaultObserver:^(id newValue) {
        GB_set_highpass_filter_mode(gb, (GB_highpass_mode_t)[newValue integerValue]);
    } forKey:@"GBHighpassFilter"];
    [self addDefaultObserver:^(id newValue) {
        GB_set_rtc_mode(gb, [newValue integerValue]);
    } forKey:@"GBRTCMode"];
    GB_apu_set_sample_callback(gb, audioCallback);
    GB_set_rumble_callback(gb, rumbleCallback);
    [self addDefaultObserver:^(id newValue) {
        GB_set_rumble_mode(gb, [newValue integerValue]);
    } forKey:@"GBRumbleMode"];
    [self addDefaultObserver:^(id newValue) {
        GB_set_interference_volume(gb, [newValue doubleValue]);
    } forKey:@"GBInterferenceVolume"];
}

- (void)addDefaultObserver:(void(^)(id newValue))block forKey:(NSString *)key
{
    if (!_defaultsObservers) {
        _defaultsObservers = [NSMutableSet set];
    }
    block = [block copy];
    [_defaultsObservers addObject:block];
    [[NSUserDefaults standardUserDefaults] addObserver:self
                                            forKeyPath:key
                                               options:NSKeyValueObservingOptionNew
                                               context:(void *)block];
    block([[NSUserDefaults standardUserDefaults] objectForKey:key]);
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context
{
    ((__bridge void(^)(id))context)(change[NSKeyValueChangeNewKey]);
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
    _gbView.hidden = true;
    _gbView.gb = &_gb;
    [_gbView screenSizeChanged];
    
    [self addDefaultObserver:^(id newValue) {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"GBFilterChanged" object:nil];
    } forKey:@"GBFilter"];
    
    __weak GBView *weakGBView = _gbView;
    [self addDefaultObserver:^(id newValue) {
        weakGBView.frameBlendingMode = [newValue integerValue];
    } forKey:@"GBFrameBlendingMode"];

    
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

- (void)loadStateFromFile:(NSString *)file
{
    GB_model_t model;
    if (!GB_get_state_model(file.fileSystemRepresentation, &model)) {
        GB_switch_model_and_reset(&_gb, model);
        GB_load_state(&_gb, file.fileSystemRepresentation);
    }
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
            [self loadStateFromFile:[GBROMManager sharedManager].autosaveStateFile];
        }
    }
    else {
        _romLoaded = false;
    }
    _gbView.hidden = !_romLoaded;
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
    UINavigationController *controller = [[UINavigationController alloc] initWithRootViewController:[[GBLoadROMTableViewController alloc] init]];
    UIBarButtonItem *close = [[UIBarButtonItem alloc] initWithTitle:@"Close"
                                                             style:UIBarButtonItemStylePlain
                                                             target:self
                                                             action:@selector(dismissViewController)];
    [controller.visibleViewController.navigationItem setLeftBarButtonItem:close];
    [self presentViewController:controller
                       animated:true
                     completion:nil];
}

- (void)changeModel
{
    GBOptionViewController *controller = [[GBOptionViewController alloc] initWithHeader:@"Select a model to emulate"];
    controller.footer = @"Changing the emulated model will reset the emulator";
    
    GB_model_t currentModel = GB_get_model(&_gb);
    struct {
        NSString *title;
        NSString *settingKey;
        bool checked;
    } items[] = {
        {@"Game Boy", @"GBDMGModel", currentModel < GB_MODEL_SGB},
        {@"Game Boy Pocket/Light", nil, currentModel == GB_MODEL_MGB},
        {@"Super Game Boy", @"GBSGBModel", GB_is_sgb(&_gb)},
        {@"Game Boy Color", @"GBCGBModel", GB_is_cgb(&_gb) && currentModel <= GB_MODEL_CGB_E},
        {@"Game Boy Advance", @"GBAGBModel", currentModel > GB_MODEL_CGB_E},
    };
    
    for (unsigned i = 0; i <  sizeof(items) / sizeof(items[0]); i++) {
        GB_model_t model = GB_MODEL_MGB;
        if (items[i].settingKey) {
            model = [[NSUserDefaults standardUserDefaults] integerForKey:items[i].settingKey];
        }
        [controller addOption:items[i].title withCheckmark:items[i].checked action:^{
            GB_switch_model_and_reset(&_gb, model);
            [self start];
        }];
    }
    [self presentViewController:controller animated:true completion:nil];
}

- (void)openStates
{
    UINavigationController *controller = [[UINavigationController alloc] initWithRootViewController:[[GBStatesViewController alloc] init]];
    UIBarButtonItem *close = [[UIBarButtonItem alloc] initWithTitle:@"Close"
                                                              style:UIBarButtonItemStylePlain
                                                             target:self
                                                             action:@selector(dismissViewController)];
    [controller.visibleViewController.navigationItem setLeftBarButtonItem:close];
    [self presentViewController:controller
                       animated:true
                     completion:nil];
}

- (void)openSettings
{
    UIBarButtonItem *close = [[UIBarButtonItem alloc] initWithTitle:@"Close"
                                                              style:UIBarButtonItemStylePlain
                                                             target:self
                                                             action:@selector(dismissViewController)];
    [self presentViewController:[GBSettingsViewController settingsViewControllerWithLeftButton:close]
                       animated:true
                     completion:nil];
}

- (void)showAbout
{
    [self presentViewController:[[GBAboutController alloc] init] animated:true completion:nil];
}

- (void)dismissViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion
{
    [super dismissViewControllerAnimated:flag completion:^() {
        if (completion) {
            completion();
        }
        if (!self.presentedViewController) {
            [self start];
        }
    }];
}

- (void)dismissViewController
{
    [self dismissViewControllerAnimated:true completion:nil];
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
    return true;
}

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
    return UIRectEdgeTop;
}

- (BOOL)prefersStatusBarHidden
{
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

- (UIImage *)imageFromData:(NSData *)data width:(unsigned)width height:(unsigned)height
{
    /* Convert the screenshot to a CGImageRef */
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data.bytes, data.length, NULL);
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaNoneSkipLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    
    CGImageRef iref = CGImageCreate(width,
                                    height,
                                    8,
                                    32,
                                    4 * width,
                                    colorSpaceRef,
                                    bitmapInfo,
                                    provider,
                                    NULL,
                                    true,
                                    renderingIntent);

    UIImage *ret = [[UIImage alloc] initWithCGImage:iref];
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
    CGImageRelease(iref);
    return ret;
}

- (void)saveStateToFile:(NSString *)file
{
    GB_save_state(&_gb, file.fileSystemRepresentation);
    NSData *data = [NSData dataWithBytes:_gbView.previousBuffer
                                  length:GB_get_screen_width(&_gb) *
                                         GB_get_screen_height(&_gb) *
                                         sizeof(*_gbView.previousBuffer)];
    UIImage *screenshot = [self imageFromData:data width:GB_get_screen_width(&_gb) height:GB_get_screen_height(&_gb)];
    [UIImagePNGRepresentation(screenshot) writeToFile:[file stringByAppendingPathExtension:@"png"] atomically:false];
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
    [self saveStateToFile:[GBROMManager sharedManager].autosaveStateFile];
    [[GBHapticManager sharedManager] setRumbleStrength:0];
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

- (const GB_palette_t *)userPalette
{
    return [GBSettingsViewController paletteForTheme:[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"]];
}

- (void)updatePalette
{
    GB_set_palette(&_gb, [self userPalette]);
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options
{
    [self stop];
    NSString *potentialROM = [[url.path stringByDeletingLastPathComponent] lastPathComponent];
    if ([[[GBROMManager sharedManager] romFileForROM:potentialROM].stringByStandardizingPath isEqualToString:url.path.stringByStandardizingPath]) {
        [GBROMManager sharedManager].currentROM = potentialROM;
    }
    else {
        [url startAccessingSecurityScopedResource];
        [GBROMManager sharedManager].currentROM =
            [[GBROMManager sharedManager] importROM:url.path
                                       keepOriginal:[options[UIApplicationOpenURLOptionsOpenInPlaceKey] boolValue]];
        [url stopAccessingSecurityScopedResource];
    }
    [self loadROM];
    [self start];
    return [GBROMManager sharedManager].currentROM != nil;
}
@end
