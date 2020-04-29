#import <IOKit/pwr_mgt/IOPMLib.h>
#import "GBView.h"
#import "GBViewGL.h"
#import "GBViewMetal.h"
#import "GBButtons.h"
#import "NSString+StringForKey.h"

#define JOYSTICK_HIGH 0x4000
#define JOYSTICK_LOW 0x3800

@implementation GBView
{
    uint32_t *image_buffers[3];
    unsigned char current_buffer;
    BOOL mouse_hidden;
    NSTrackingArea *tracking_area;
    BOOL _mouseHidingEnabled;
    bool axisActive[2];
    bool underclockKeyDown;
    double clockMultiplier;
    double analogClockMultiplier;
    bool analogClockMultiplierValid;
    NSEventModifierFlags previousModifiers;
    JOYController *lastController;
    GB_frame_blending_mode_t _frameBlendingMode;
}

+ (instancetype)alloc
{
    return [self allocWithZone:NULL];
}

+ (instancetype)allocWithZone:(struct _NSZone *)zone
{
    if (self == [GBView class]) {
        if ([GBViewMetal isSupported]) {
            return [GBViewMetal allocWithZone: zone];
        }
        return [GBViewGL allocWithZone: zone];
    }
    return [super allocWithZone:zone];
}

- (void) createInternalView
{
    assert(false && "createInternalView must not be inherited");
}

- (void) _init
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ratioKeepingChanged) name:@"GBAspectChanged" object:nil];
    tracking_area = [ [NSTrackingArea alloc] initWithRect:(NSRect){}
                                                  options:NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways | NSTrackingInVisibleRect
                                                    owner:self
                                                 userInfo:nil];
    [self addTrackingArea:tracking_area];
    clockMultiplier = 1.0;
    [self createInternalView];
    [self addSubview:self.internalView];
    self.internalView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [JOYController registerListener:self];
}

- (void)screenSizeChanged
{
    if (image_buffers[0]) free(image_buffers[0]);
    if (image_buffers[1]) free(image_buffers[1]);
    if (image_buffers[2]) free(image_buffers[2]);
    
    size_t buffer_size = sizeof(image_buffers[0][0]) * GB_get_screen_width(_gb) * GB_get_screen_height(_gb);
    
    image_buffers[0] = calloc(1, buffer_size);
    image_buffers[1] = calloc(1, buffer_size);
    image_buffers[2] = calloc(1, buffer_size);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setFrame:self.superview.frame];
    });
}

- (void) ratioKeepingChanged
{
    [self setFrame:self.superview.frame];
}

- (void) setFrameBlendingMode:(GB_frame_blending_mode_t)frameBlendingMode
{
    _frameBlendingMode = frameBlendingMode;
    [self setNeedsDisplay:YES];
}


- (GB_frame_blending_mode_t)frameBlendingMode
{
    if (_frameBlendingMode == GB_FRAME_BLENDING_MODE_ACCURATE) {
        if (!_gb || GB_is_sgb(_gb)) {
            return GB_FRAME_BLENDING_MODE_SIMPLE;
        }
        return GB_is_odd_frame(_gb)? GB_FRAME_BLENDING_MODE_ACCURATE_ODD : GB_FRAME_BLENDING_MODE_ACCURATE_EVEN;
    }
    return _frameBlendingMode;
}
- (unsigned char) numberOfBuffers
{
    return _frameBlendingMode? 3 : 2;
}

- (void)dealloc
{
    free(image_buffers[0]);
    free(image_buffers[1]);
    free(image_buffers[2]);
    if (mouse_hidden) {
        mouse_hidden = false;
        [NSCursor unhide];
    }
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [lastController setRumbleAmplitude:0];
    [JOYController unregisterListener:self];
}
- (instancetype)initWithCoder:(NSCoder *)coder
{
    if (!(self = [super initWithCoder:coder])) { 
        return self;
    }
    [self _init];
    return self;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (!(self = [super initWithFrame:frameRect])) { 
        return self;
    }
    [self _init];
    return self;
}

- (void)setFrame:(NSRect)frame
{
    frame = self.superview.frame;
    if (_gb && ![[NSUserDefaults standardUserDefaults] boolForKey:@"GBAspectRatioUnkept"]) {
        double ratio = frame.size.width / frame.size.height;
        double width = GB_get_screen_width(_gb);
        double height = GB_get_screen_height(_gb);
        if (ratio >= width / height) {
            double new_width = round(frame.size.height / height * width);
            frame.origin.x = floor((frame.size.width - new_width) / 2);
            frame.size.width = new_width;
            frame.origin.y = 0;
        }
        else {
            double new_height = round(frame.size.width / width * height);
            frame.origin.y = floor((frame.size.height - new_height) / 2);
            frame.size.height = new_height;
            frame.origin.x = 0;
        }
    }

    [super setFrame:frame];
}

- (void) flip
{
    if (analogClockMultiplierValid && [[NSUserDefaults standardUserDefaults] boolForKey:@"GBAnalogControls"]) {
        GB_set_clock_multiplier(_gb, analogClockMultiplier);
        if (analogClockMultiplier == 1.0) {
            analogClockMultiplierValid = false;
        }
    }
    else {
        if (underclockKeyDown && clockMultiplier > 0.5) {
            clockMultiplier -= 1.0/16;
            GB_set_clock_multiplier(_gb, clockMultiplier);
        }
        if (!underclockKeyDown && clockMultiplier < 1.0) {
            clockMultiplier += 1.0/16;
            GB_set_clock_multiplier(_gb, clockMultiplier);
        }
    }
    current_buffer = (current_buffer + 1) % self.numberOfBuffers;
}

- (uint32_t *) pixels
{
    return image_buffers[(current_buffer + 1) % self.numberOfBuffers];
}

-(void)keyDown:(NSEvent *)theEvent
{
    unsigned short keyCode = theEvent.keyCode;
    bool handled = false;

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    unsigned player_count = GB_get_player_count(_gb);
    for (unsigned player = 0; player < player_count; player++) {
        for (GBButton button = 0; button < GBButtonCount; button++) {
            NSNumber *key = [defaults valueForKey:button_to_preference_name(button, player)];
            if (!key) continue;

            if (key.unsignedShortValue == keyCode) {
                handled = true;
                switch (button) {
                    case GBTurbo:
                        GB_set_turbo_mode(_gb, true, self.isRewinding);
                        analogClockMultiplierValid = false;
                        break;
                        
                    case GBRewind:
                        self.isRewinding = true;
                        GB_set_turbo_mode(_gb, false, false);
                        break;
                        
                    case GBUnderclock:
                        underclockKeyDown = true;
                        analogClockMultiplierValid = false;
                        break;
                        
                    default:
                        GB_set_key_state_for_player(_gb, (GB_key_t)button, player, true);
                        break;
                }
            }
        }
    }

    if (!handled && [theEvent type] != NSEventTypeFlagsChanged) {
        [super keyDown:theEvent];
    }
}

-(void)keyUp:(NSEvent *)theEvent
{
    unsigned short keyCode = theEvent.keyCode;
    bool handled = false;

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    unsigned player_count = GB_get_player_count(_gb);
    for (unsigned player = 0; player < player_count; player++) {
        for (GBButton button = 0; button < GBButtonCount; button++) {
            NSNumber *key = [defaults valueForKey:button_to_preference_name(button, player)];
            if (!key) continue;
            
            if (key.unsignedShortValue == keyCode) {
                handled = true;
                switch (button) {
                    case GBTurbo:
                        GB_set_turbo_mode(_gb, false, false);
                        analogClockMultiplierValid = false;
                        break;
                        
                    case GBRewind:
                        self.isRewinding = false;
                        break;
                        
                    case GBUnderclock:
                        underclockKeyDown = false;
                        analogClockMultiplierValid = false;
                        break;
                        
                    default:
                        GB_set_key_state_for_player(_gb, (GB_key_t)button, player, false);
                        break;
                }
            }
        }
    }
    if (!handled && [theEvent type] != NSEventTypeFlagsChanged) {
        [super keyUp:theEvent];
    }
}

- (void)setRumble:(bool)on
{
    [lastController setRumbleAmplitude:(double)on];
}

- (void)controller:(JOYController *)controller movedAxis:(JOYAxis *)axis
{
    if (![self.window isMainWindow]) return;

    NSDictionary *mapping = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitInstanceMapping"][controller.uniqueID];
    if (!mapping) {
        mapping = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitNameMapping"][controller.deviceName];
    }
    
    if ((axis.usage == JOYAxisUsageR1 && !mapping) ||
        axis.uniqueID == [mapping[@"AnalogUnderclock"] unsignedLongValue]){
        analogClockMultiplier = MIN(MAX(1 - axis.value + 0.2, 1.0 / 3), 1.0);
        analogClockMultiplierValid = true;
    }
    
    else if ((axis.usage == JOYAxisUsageL1 && !mapping) ||
        axis.uniqueID == [mapping[@"AnalogTurbo"] unsignedLongValue]){
        analogClockMultiplier = MIN(MAX(axis.value * 3 + 0.8, 1.0), 3.0);
        analogClockMultiplierValid = true;
    }
}

- (void)controller:(JOYController *)controller buttonChangedState:(JOYButton *)button
{
    if (![self.window isMainWindow]) return;
    if (controller != lastController) {
        [lastController setRumbleAmplitude:0];
        lastController = controller;
    }
    
    
    unsigned player_count = GB_get_player_count(_gb);

    IOPMAssertionID assertionID;
    IOPMAssertionDeclareUserActivity(CFSTR(""), kIOPMUserActiveLocal, &assertionID);
    
    for (unsigned player = 0; player < player_count; player++) {
        NSString *preferred_joypad = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitDefaultControllers"]
                                      objectForKey:n2s(player)];
        if (player_count != 1 && // Single player, accpet inputs from all joypads
            !(player == 0 && !preferred_joypad) && // Multiplayer, but player 1 has no joypad configured, so it takes inputs from all joypads
            ![preferred_joypad isEqualToString:controller.uniqueID]) {
            continue;
        }
        [controller setPlayerLEDs:1 << player];
        NSDictionary *mapping = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitInstanceMapping"][controller.uniqueID];
        if (!mapping) {
            mapping = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"JoyKitNameMapping"][controller.deviceName];
        }
        
        JOYButtonUsage usage = ((JOYButtonUsage)[mapping[n2s(button.uniqueID)] unsignedIntValue]) ?: button.usage;
        if (!mapping && usage >= JOYButtonUsageGeneric0) {
            usage = (const JOYButtonUsage[]){JOYButtonUsageY, JOYButtonUsageA, JOYButtonUsageB, JOYButtonUsageX}[(usage - JOYButtonUsageGeneric0) & 3];
        }
        
        switch (usage) {
                
            case JOYButtonUsageNone: break;
            case JOYButtonUsageA: GB_set_key_state_for_player(_gb, GB_KEY_A, player, button.isPressed); break;
            case JOYButtonUsageB: GB_set_key_state_for_player(_gb, GB_KEY_B, player, button.isPressed); break;
            case JOYButtonUsageC: break;
            case JOYButtonUsageStart:
            case JOYButtonUsageX: GB_set_key_state_for_player(_gb, GB_KEY_START, player, button.isPressed); break;
            case JOYButtonUsageSelect:
            case JOYButtonUsageY: GB_set_key_state_for_player(_gb, GB_KEY_SELECT, player, button.isPressed); break;
            case JOYButtonUsageR2:
            case JOYButtonUsageL2:
            case JOYButtonUsageZ: {
                self.isRewinding = button.isPressed;
                if (button.isPressed) {
                    GB_set_turbo_mode(_gb, false, false);
                }
                break;
            }
        
            case JOYButtonUsageL1: GB_set_turbo_mode(_gb, button.isPressed, button.isPressed && self.isRewinding); break;

            case JOYButtonUsageR1: underclockKeyDown = button.isPressed; break;
            case JOYButtonUsageDPadLeft: GB_set_key_state_for_player(_gb, GB_KEY_LEFT, player, button.isPressed); break;
            case JOYButtonUsageDPadRight: GB_set_key_state_for_player(_gb, GB_KEY_RIGHT, player, button.isPressed); break;
            case JOYButtonUsageDPadUp: GB_set_key_state_for_player(_gb, GB_KEY_UP, player, button.isPressed); break;
            case JOYButtonUsageDPadDown: GB_set_key_state_for_player(_gb, GB_KEY_DOWN, player, button.isPressed); break;

            default:
                break;
        }
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)mouseEntered:(NSEvent *)theEvent
{
    if (!mouse_hidden) {
        mouse_hidden = true;
        if (_mouseHidingEnabled) {
            [NSCursor hide];
        }
    }
    [super mouseEntered:theEvent];
}

- (void)mouseExited:(NSEvent *)theEvent
{
    if (mouse_hidden) {
        mouse_hidden = false;
        if (_mouseHidingEnabled) {
            [NSCursor unhide];
        }
    }
    [super mouseExited:theEvent];
}

- (void)setMouseHidingEnabled:(BOOL)mouseHidingEnabled
{
    if (mouseHidingEnabled == _mouseHidingEnabled) return;

    _mouseHidingEnabled = mouseHidingEnabled;
    
    if (mouse_hidden && _mouseHidingEnabled) {
        [NSCursor hide];
    }

    if (mouse_hidden && !_mouseHidingEnabled) {
        [NSCursor unhide];
    }
}

- (BOOL)isMouseHidingEnabled
{
    return _mouseHidingEnabled;
}

- (void) flagsChanged:(NSEvent *)event
{
    if (event.modifierFlags > previousModifiers) {
        [self keyDown:event];
    }
    else {
        [self keyUp:event];
    }
    
    previousModifiers = event.modifierFlags;
}

- (uint32_t *)currentBuffer
{
    return image_buffers[current_buffer];
}

- (uint32_t *)previousBuffer
{
    return image_buffers[(current_buffer + 2) % self.numberOfBuffers];
}

@end
