#import "JOYController.h"
#import "JOYMultiplayerController.h"
#import "JOYElement.h"
#import "JOYSubElement.h"
#import "JOYEmulatedButton.h"
#include <IOKit/hid/IOHIDLib.h>

#define PWM_RESOLUTION 16

static NSString const *JOYAxisGroups = @"JOYAxisGroups";
static NSString const *JOYReportIDFilters = @"JOYReportIDFilters";
static NSString const *JOYButtonUsageMapping = @"JOYButtonUsageMapping";
static NSString const *JOYAxisUsageMapping = @"JOYAxisUsageMapping";
static NSString const *JOYAxes2DUsageMapping = @"JOYAxes2DUsageMapping";
static NSString const *JOYSubElementStructs = @"JOYSubElementStructs";
static NSString const *JOYIsSwitch = @"JOYIsSwitch";
static NSString const *JOYRumbleUsage = @"JOYRumbleUsage";
static NSString const *JOYRumbleUsagePage = @"JOYRumbleUsagePage";
static NSString const *JOYConnectedUsage = @"JOYConnectedUsage";
static NSString const *JOYConnectedUsagePage = @"JOYConnectedUsagePage";
static NSString const *JOYRumbleMin = @"JOYRumbleMin";
static NSString const *JOYRumbleMax = @"JOYRumbleMax";
static NSString const *JOYSwapZRz = @"JOYSwapZRz";
static NSString const *JOYActivationReport = @"JOYActivationReport";

static NSMutableDictionary<id, JOYController *> *controllers; // Physical controllers
static NSMutableArray<JOYController *> *exposedControllers; // Logical controllers

static NSDictionary *hacksByName = nil;
static NSDictionary *hacksByManufacturer = nil;

static NSMutableSet<id<JOYListener>> *listeners = nil;

static bool axesEmulateButtons = false;
static bool axes2DEmulateButtons = false;
static bool hatsEmulateButtons = false;

@interface JOYController ()
+ (void)controllerAdded:(IOHIDDeviceRef) device;
+ (void)controllerRemoved:(IOHIDDeviceRef) device;
- (void)elementChanged:(IOHIDElementRef) element;
@end

@interface JOYButton ()
- (instancetype)initWithElement:(JOYElement *)element;
- (bool)updateState;
@end

@interface JOYAxis ()
- (instancetype)initWithElement:(JOYElement *)element;
- (bool)updateState;
@end

@interface JOYHat ()
- (instancetype)initWithElement:(JOYElement *)element;
- (bool)updateState;
@end

@interface JOYAxes2D ()
- (instancetype)initWithFirstElement:(JOYElement *)element1 secondElement:(JOYElement *)element2;
- (bool)updateState;
@end

static NSDictionary *CreateHIDDeviceMatchDictionary(const UInt32 page, const UInt32 usage)
{
    return @{
        @kIOHIDDeviceUsagePageKey: @(page),
        @kIOHIDDeviceUsageKey: @(usage),
    };
}

static void HIDDeviceAdded(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    [JOYController controllerAdded:device];
}

static void HIDDeviceRemoved(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    [JOYController controllerRemoved:device];
}

static void HIDInput(void *context, IOReturn result, void *sender, IOHIDValueRef value)
{
    [(__bridge JOYController *)context elementChanged:IOHIDValueGetElement(value)];
}


typedef struct __attribute__((packed)) {
    uint8_t reportID;
    uint8_t sequence;
    uint8_t rumbleData[8];
    uint8_t command;
    uint8_t commandData[26];
} JOYSwitchPacket;

@implementation JOYController
{
    IOHIDDeviceRef _device;
    NSMutableDictionary<JOYElement *, JOYButton *> *_buttons;
    NSMutableDictionary<JOYElement *, JOYAxis *> *_axes;
    NSMutableDictionary<JOYElement *, JOYAxes2D *> *_axes2D;
    NSMutableDictionary<JOYElement *, JOYHat *> *_hats;
    NSMutableDictionary<JOYElement *, NSArray<JOYElement *> *> *_multiElements;

    // Button emulation
    NSMutableDictionary<NSNumber *, JOYEmulatedButton *> *_axisEmulatedButtons;
    NSMutableDictionary<NSNumber *, NSArray <JOYEmulatedButton *> *> *_axes2DEmulatedButtons;
    NSMutableDictionary<NSNumber *, NSArray <JOYEmulatedButton *> *> *_hatEmulatedButtons;

    JOYElement *_rumbleElement;
    JOYElement *_connectedElement;
    NSMutableDictionary<NSValue *, JOYElement *> *_iokitToJOY;
    NSString *_serialSuffix;
    bool _isSwitch; // Does thie controller use the Switch protocol?
    JOYSwitchPacket _lastSwitchPacket;
    NSCondition *_rumblePWMThreadLock;
    volatile double _rumblePWMRatio;
    bool _physicallyConnected;
    bool _logicallyConnected;
    bool _rumblePWMThreadRunning;
    volatile bool _forceStopPWMThread;
}

- (instancetype)initWithDevice:(IOHIDDeviceRef) device
{
    return [self initWithDevice:device reportIDFilter:nil serialSuffix:nil];
}

- (instancetype)initWithDevice:(IOHIDDeviceRef)device reportIDFilter:(NSArray <NSNumber *> *) filter serialSuffix:(NSString *)suffix
{
    self = [super init];
    if (!self) return self;
    
    _physicallyConnected = true;
    _logicallyConnected = true;
    _device = (IOHIDDeviceRef)CFRetain(device);
    _serialSuffix = suffix;

    IOHIDDeviceRegisterInputValueCallback(device, HIDInput, (void *)self);
    IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    NSArray *array = CFBridgingRelease(IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone));
    _buttons = [NSMutableDictionary dictionary];
    _axes = [NSMutableDictionary dictionary];
    _axes2D = [NSMutableDictionary dictionary];
    _hats = [NSMutableDictionary dictionary];
    _axisEmulatedButtons = [NSMutableDictionary dictionary];
    _axes2DEmulatedButtons = [NSMutableDictionary dictionary];
    _hatEmulatedButtons = [NSMutableDictionary dictionary];
    _multiElements = [NSMutableDictionary dictionary];
    _iokitToJOY = [NSMutableDictionary dictionary];
    _rumblePWMThreadLock = [[NSCondition alloc] init];
    
    
    //NSMutableArray *axes3d = [NSMutableArray array];
    
    NSDictionary *axisGroups = @{
        @(kHIDUsage_GD_X): @(0),
        @(kHIDUsage_GD_Y): @(0),
        @(kHIDUsage_GD_Z): @(1),
        @(kHIDUsage_GD_Rx): @(2),
        @(kHIDUsage_GD_Ry): @(2),
        @(kHIDUsage_GD_Rz): @(1),
    };
    NSString *name = (__bridge NSString *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
    NSDictionary *hacks = hacksByName[name];
    if (!hacks) {
        hacks = hacksByManufacturer[(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey))];
    }
    axisGroups = hacks[JOYAxisGroups] ?: axisGroups;
    _isSwitch = [hacks[JOYIsSwitch] boolValue];
    uint16_t rumbleUsagePage = (uint16_t)[hacks[JOYRumbleUsagePage] unsignedIntValue];
    uint16_t rumbleUsage = (uint16_t)[hacks[JOYRumbleUsage] unsignedIntValue];
    uint16_t connectedUsagePage = (uint16_t)[hacks[JOYConnectedUsagePage] unsignedIntValue];
    uint16_t connectedUsage = (uint16_t)[hacks[JOYConnectedUsage] unsignedIntValue];

    JOYElement *previousAxisElement = nil;
    id previous = nil;
    for (id _element in array) {
        if (_element == previous) continue; // Some elements are reported twice for some reason
        previous = _element;
        NSArray *elements = nil;
        JOYElement *element = [[JOYElement alloc] initWithElement:(__bridge IOHIDElementRef)_element];
        
        NSArray<NSDictionary <NSString *,NSNumber *>*> *subElementDefs = hacks[JOYSubElementStructs][@(element.uniqueID)];

        bool isOutput = false;
        if (subElementDefs && element.uniqueID != element.parentID) {
            elements = [NSMutableArray array];
            for (NSDictionary<NSString *,NSNumber *> *virtualInput in subElementDefs) {
                if (filter && virtualInput[@"reportID"] && ![filter containsObject:virtualInput[@"reportID"]]) continue;
                [(NSMutableArray *)elements addObject:[[JOYSubElement alloc] initWithRealElement:element
                                                                                            size:virtualInput[@"size"].unsignedLongValue
                                                                                          offset:virtualInput[@"offset"].unsignedLongValue
                                                                                       usagePage:virtualInput[@"usagePage"].unsignedLongValue
                                                                                           usage:virtualInput[@"usage"].unsignedLongValue
                                                                                             min:virtualInput[@"min"].unsignedIntValue
                                                                                             max:virtualInput[@"max"].unsignedIntValue]];
            }
            isOutput = IOHIDElementGetType((__bridge IOHIDElementRef)_element) == kIOHIDElementTypeOutput;
            [_multiElements setObject:elements forKey:element];
        }
        else {
            if (filter && ![filter containsObject:@(element.reportID)]) continue;

            switch (IOHIDElementGetType((__bridge IOHIDElementRef)_element)) {
                /* Handled */
                case kIOHIDElementTypeInput_Misc:
                case kIOHIDElementTypeInput_Button:
                case kIOHIDElementTypeInput_Axis:
                    break;
                case kIOHIDElementTypeOutput:
                    isOutput = true;
                    break;
                /* Ignored */
                default:
                case kIOHIDElementTypeInput_ScanCodes:
                case kIOHIDElementTypeInput_NULL:
                case kIOHIDElementTypeFeature:
                case kIOHIDElementTypeCollection:
                    continue;
            }
            if (IOHIDElementIsArray((__bridge IOHIDElementRef)_element)) continue;
            
            elements = @[element];
        }
        
        _iokitToJOY[@(IOHIDElementGetCookie((__bridge IOHIDElementRef)_element))] = element;
        
        for (JOYElement *element in elements) {
            if (isOutput) {
                if (!_rumbleElement && rumbleUsage && rumbleUsagePage && element.usage == rumbleUsage && element.usagePage == rumbleUsagePage) {
                    if (hacks[JOYRumbleMin]) {
                        element.min = [hacks[JOYRumbleMin] unsignedIntValue];
                    }
                    if (hacks[JOYRumbleMax]) {
                        element.max = [hacks[JOYRumbleMax] unsignedIntValue];
                    }
                    _rumbleElement = element;
                }
                continue;
            }
            else {
                if (!_connectedElement && connectedUsage && connectedUsagePage && element.usage == connectedUsage && element.usagePage == connectedUsagePage) {
                    _connectedElement = element;
                    _logicallyConnected = element.value != element.min;
                    continue;
                }
            }
            
            if (element.usagePage == kHIDPage_Button) {
                button: {
                    JOYButton *button = [[JOYButton alloc] initWithElement: element];
                    [_buttons setObject:button forKey:element];
                    NSNumber *replacementUsage = hacks[JOYButtonUsageMapping][@(button.usage)];
                    if (replacementUsage) {
                        button.usage = [replacementUsage unsignedIntValue];
                    }
                    continue;
                }
            }
            else if (element.usagePage == kHIDPage_GenericDesktop) {
                switch (element.usage) {
                    case kHIDUsage_GD_X:
                    case kHIDUsage_GD_Y:
                    case kHIDUsage_GD_Z:
                    case kHIDUsage_GD_Rx:
                    case kHIDUsage_GD_Ry:
                    case kHIDUsage_GD_Rz: {
                        
                        JOYElement *other = previousAxisElement;
                        previousAxisElement = element;
                        if (!other) goto single;
                        if (other.usage >= element.usage) goto single;
                        if (other.reportID != element.reportID) goto single;
                        if (![axisGroups[@(other.usage)] isEqualTo: axisGroups[@(element.usage)]]) goto single;
                        if (other.parentID != element.parentID) goto single;
                        
                        JOYAxes2D *axes = nil;
                        if (other.usage == kHIDUsage_GD_Z && element.usage == kHIDUsage_GD_Rz && [hacks[JOYSwapZRz] boolValue]) {
                            axes = [[JOYAxes2D alloc] initWithFirstElement:element secondElement:other];
                        }
                        else {
                            axes = [[JOYAxes2D alloc] initWithFirstElement:other secondElement:element];
                        }
                        NSNumber *replacementUsage = hacks[JOYAxes2DUsageMapping][@(axes.usage)];
                        if (replacementUsage) {
                            axes.usage = [replacementUsage unsignedIntValue];
                        }

                        [_axisEmulatedButtons removeObjectForKey:@(_axes[other].uniqueID)];
                        [_axes removeObjectForKey:other];
                        previousAxisElement = nil;
                        _axes2D[other] = axes;
                        _axes2D[element] = axes;
                        
                        if (axes2DEmulateButtons) {
                            _axes2DEmulatedButtons[@(axes.uniqueID)] = @[
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadLeft  uniqueID:axes.uniqueID | 0x100000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadRight uniqueID:axes.uniqueID | 0x200000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadUp  uniqueID:axes.uniqueID | 0x300000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadDown uniqueID:axes.uniqueID | 0x400000000L],
                                ];
                        }
                        
                        /*
                        for (NSArray *group in axes2d) {
                            break;
                            IOHIDElementRef first  = (__bridge IOHIDElementRef)group[0];
                            IOHIDElementRef second = (__bridge IOHIDElementRef)group[1];
                            if (IOHIDElementGetUsage(first)  > element.usage) continue;
                            if (IOHIDElementGetUsage(second) > element.usage) continue;
                            if (IOHIDElementGetReportID(first) != IOHIDElementGetReportID(element)) continue;
                            if ((IOHIDElementGetUsage(first) - kHIDUsage_GD_X) / 3 != (element.usage - kHIDUsage_GD_X) / 3) continue;
                            if (IOHIDElementGetParent(first) != IOHIDElementGetParent(element)) continue;
                            
                            [axes2d removeObject:group];
                            [axes3d addObject:@[(__bridge id)first, (__bridge id)second, _element]];
                            found = true;
                            break;
                        }*/
                        break;
                    }
                    single:
                    case kHIDUsage_GD_Slider:
                    case kHIDUsage_GD_Dial:
                    case kHIDUsage_GD_Wheel: {
                        JOYAxis *axis = [[JOYAxis alloc] initWithElement: element];
                        [_axes setObject:axis forKey:element];

                        NSNumber *replacementUsage = hacks[JOYAxisUsageMapping][@(axis.usage)];
                        if (replacementUsage) {
                            axis.usage = [replacementUsage unsignedIntValue];
                        }
                        
                        if (axesEmulateButtons && axis.usage >= JOYAxisUsageL1 && axis.usage <= JOYAxisUsageR3) {
                            _axisEmulatedButtons[@(axis.uniqueID)] =
                                [[JOYEmulatedButton alloc] initWithUsage:axis.usage - JOYAxisUsageL1 + JOYButtonUsageL1 uniqueID:axis.uniqueID];
                        }
                        
                        if (axesEmulateButtons && axis.usage >= JOYAxisUsageGeneric0) {
                            _axisEmulatedButtons[@(axis.uniqueID)] =
                                [[JOYEmulatedButton alloc] initWithUsage:axis.usage - JOYAxisUsageGeneric0 + JOYButtonUsageGeneric0 uniqueID:axis.uniqueID];
                        }

                        break;
                    }
                    case kHIDUsage_GD_DPadUp:
                    case kHIDUsage_GD_DPadDown:
                    case kHIDUsage_GD_DPadRight:
                    case kHIDUsage_GD_DPadLeft:
                    case kHIDUsage_GD_Start:
                    case kHIDUsage_GD_Select:
                    case kHIDUsage_GD_SystemMainMenu:
                        goto button;
                        
                    case kHIDUsage_GD_Hatswitch: {
                        JOYHat *hat = [[JOYHat alloc] initWithElement: element];
                        [_hats setObject:hat forKey:element];
                        if (hatsEmulateButtons) {
                            _hatEmulatedButtons[@(hat.uniqueID)] = @[
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadLeft  uniqueID:hat.uniqueID | 0x100000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadRight uniqueID:hat.uniqueID | 0x200000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadUp  uniqueID:hat.uniqueID | 0x300000000L],
                                    [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadDown uniqueID:hat.uniqueID | 0x400000000L],
                                ];
                        }
                        break;
                    }
                }
            }
        }
    }
    
    [exposedControllers addObject:self];
    if (_logicallyConnected) {
        for (id<JOYListener> listener in listeners) {
            if ([listener respondsToSelector:@selector(controllerConnected:)]) {
                [listener controllerConnected:self];
            }
        }
    }
    
    return self;
}

- (NSString *)deviceName
{
    if (!_device) return nil;
    return IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDProductKey));
}

- (NSString *)uniqueID
{
    if (!_device) return nil;
    NSString *serial = (__bridge NSString *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDSerialNumberKey));
    if (!serial || [(__bridge NSString *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDTransportKey)) isEqualToString:@"USB"]) {
        serial = [NSString stringWithFormat:@"%04x%04x%08x",
                  [(__bridge NSNumber *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDVendorIDKey)) unsignedIntValue],
                  [(__bridge NSNumber *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDProductIDKey)) unsignedIntValue],
                  [(__bridge NSNumber *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDLocationIDKey)) unsignedIntValue]];
    }
    if (_serialSuffix) {
        return [NSString stringWithFormat:@"%@-%@", serial, _serialSuffix];
    }
    return serial;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@: %p, %@, %@>", self.className, self, self.deviceName, self.uniqueID];
}

- (NSArray<JOYButton *> *)buttons
{
    NSMutableArray *ret = [[_buttons allValues] mutableCopy];
    [ret addObjectsFromArray:_axisEmulatedButtons.allValues];
    for (NSArray *array in _axes2DEmulatedButtons.allValues) {
        [ret addObjectsFromArray:array];
    }
    for (NSArray *array in _hatEmulatedButtons.allValues) {
        [ret addObjectsFromArray:array];
    }
    return ret;
}

- (NSArray<JOYAxis *> *)axes
{
    return [_axes allValues];
}

- (NSArray<JOYAxes2D *> *)axes2D
{
    return [[NSSet setWithArray:[_axes2D allValues]] allObjects];
}

- (NSArray<JOYHat *> *)hats
{
    return [_hats allValues];
}

- (void)elementChanged:(IOHIDElementRef)element
{
    JOYElement *_element = _iokitToJOY[@(IOHIDElementGetCookie(element))];
    if (_element) {
        [self _elementChanged:_element];
    }
    else {
        //NSLog(@"Unhandled usage %x (Cookie: %x, Usage: %x)", IOHIDElementGetUsage(element), IOHIDElementGetCookie(element), IOHIDElementGetUsage(element));
    }
}

- (void)_elementChanged:(JOYElement *)element
{
    if (element == _connectedElement) {
        bool old = self.connected;
        _logicallyConnected = _connectedElement.value != _connectedElement.min;
        if (!old && self.connected) {
            for (id<JOYListener> listener in listeners) {
                if ([listener respondsToSelector:@selector(controllerConnected:)]) {
                    [listener controllerConnected:self];
                }
            }
        }
        else if (old && !self.connected) {
            for (id<JOYListener> listener in listeners) {
                if ([listener respondsToSelector:@selector(controllerDisconnected:)]) {
                    [listener controllerDisconnected:self];
                }
            }
        }
    }
    
    {
        NSArray<JOYElement *> *subElements = _multiElements[element];
        if (subElements) {
            for (JOYElement *subElement in subElements) {
                [self _elementChanged:subElement];
            }
            return;
        }
    }
    
    if (!self.connected) return;
    {
        JOYButton *button = _buttons[element];
        if (button) {
            if ([button updateState]) {
                for (id<JOYListener> listener in listeners) {
                    if ([listener respondsToSelector:@selector(controller:buttonChangedState:)]) {
                        [listener controller:self buttonChangedState:button];
                    }
                }
            }
            return;
        }
    }
    

    {
        JOYAxis *axis = _axes[element];
        if (axis) {
            if ([axis updateState])  {
                for (id<JOYListener> listener in listeners) {
                    if ([listener respondsToSelector:@selector(controller:movedAxis:)]) {
                        [listener controller:self movedAxis:axis];
                    }
                }
                JOYEmulatedButton *button = _axisEmulatedButtons[@(axis.uniqueID)];
                if ([button updateStateFromAxis:axis]) {
                    for (id<JOYListener> listener in listeners) {
                        if ([listener respondsToSelector:@selector(controller:buttonChangedState:)]) {
                            [listener controller:self buttonChangedState:button];
                        }
                    }
                }
            }
            return;
        }
    }
    
    {
        JOYAxes2D *axes = _axes2D[element];
        if (axes) {
            if ([axes updateState]) {
                for (id<JOYListener> listener in listeners) {
                    if ([listener respondsToSelector:@selector(controller:movedAxes2D:)]) {
                        [listener controller:self movedAxes2D:axes];
                    }
                }
                NSArray <JOYEmulatedButton *> *buttons = _axes2DEmulatedButtons[@(axes.uniqueID)];
                for (JOYEmulatedButton *button in buttons) {
                    if ([button updateStateFromAxes2D:axes]) {
                        for (id<JOYListener> listener in listeners) {
                            if ([listener respondsToSelector:@selector(controller:buttonChangedState:)]) {
                                [listener controller:self buttonChangedState:button];
                            }
                        }
                    }
                }
            }
            return;
        }
    }
    
    {
        JOYHat *hat = _hats[element];
        if (hat) {
            if ([hat updateState]) {
                for (id<JOYListener> listener in listeners) {
                    if ([listener respondsToSelector:@selector(controller:movedHat:)]) {
                        [listener controller:self movedHat:hat];
                    }
                }
                
                NSArray <JOYEmulatedButton *> *buttons = _hatEmulatedButtons[@(hat.uniqueID)];
                for (JOYEmulatedButton *button in buttons) {
                    if ([button updateStateFromHat:hat]) {
                        for (id<JOYListener> listener in listeners) {
                            if ([listener respondsToSelector:@selector(controller:buttonChangedState:)]) {
                                [listener controller:self buttonChangedState:button];
                            }
                        }
                    }
                }
            }
            return;
        }
    }
}

- (void)disconnected
{
    if (_logicallyConnected && [exposedControllers containsObject:self]) {
        for (id<JOYListener> listener in listeners) {
            if ([listener respondsToSelector:@selector(controllerDisconnected:)]) {
                [listener controllerDisconnected:self];
            }
        }
    }
    _physicallyConnected = false;
    [self setRumbleAmplitude:0]; // Stop the rumble thread.
    [exposedControllers removeObject:self];
    _device = nil;
}

- (void)sendReport:(NSData *)report
{
    if (!report.length) return;
    if (!_device) return;
    IOHIDDeviceSetReport(_device, kIOHIDReportTypeOutput, *(uint8_t *)report.bytes, report.bytes, report.length);
}

- (void)setPlayerLEDs:(uint8_t)mask
{
    mask &= 0xF;
    if (_isSwitch) {
        _lastSwitchPacket.reportID = 0x1; // Rumble and LEDs
        _lastSwitchPacket.sequence++;
        _lastSwitchPacket.sequence &= 0xF;
        _lastSwitchPacket.command = 0x30; // LED
        _lastSwitchPacket.commandData[0] = mask;
        [self sendReport:[NSData dataWithBytes:&_lastSwitchPacket length:sizeof(_lastSwitchPacket)]];
    }
}

- (void)pwmThread
{
    /* TODO: This does not handle correctly the case of having a multi-port controller where more than one controller
             uses rumble. At least make sure any sibling controllers don't have their PWM thread running. */
    for (JOYController *controller in [JOYController allControllers]) {
        if (controller != self && controller->_device == _device) {
            [controller _forceStopPWMThread];
        }
    }
    unsigned rumbleCounter = 0;
    while (self.connected && !_forceStopPWMThread) {
        if ([_rumbleElement setValue:rumbleCounter < round(_rumblePWMRatio * PWM_RESOLUTION)]) {
            break;
        }
        rumbleCounter += round(_rumblePWMRatio * PWM_RESOLUTION);
        if (rumbleCounter >= PWM_RESOLUTION) {
            rumbleCounter -= PWM_RESOLUTION;
        }
    }
    [_rumblePWMThreadLock lock];
    [_rumbleElement setValue:0];
    _rumblePWMThreadRunning = false;
    _forceStopPWMThread = false;
    [_rumblePWMThreadLock unlock];
}

- (void)setRumbleAmplitude:(double)amp /* andFrequency: (double)frequency */
{
    double frequency = 144; // I have no idea what I'm doing.
    
    if (amp < 0) amp = 0;
    if (amp > 1) amp = 1;
    if (_isSwitch) {
        if (amp == 0) {
            amp = 1;
            frequency = 0;
        }
        
        uint8_t highAmp = amp * 0x64;
        uint8_t lowAmp = amp * 0x32 + 0x40;
        if (frequency < 0) frequency = 0;
        if (frequency > 1252) frequency = 1252;
        uint8_t encodedFrequency = (uint8_t)round(log2(frequency / 10.0) * 32.0);

        uint16_t highFreq = (encodedFrequency - 0x60) * 4;
        uint8_t lowFreq = encodedFrequency - 0x40;
        
        //if (frequency < 82 || frequency > 312) {
        if (amp) {
            highAmp = 0;
        }
        
        if (frequency < 40 || frequency > 626) {
            lowAmp = 0;
        }
        
        _lastSwitchPacket.rumbleData[0] = _lastSwitchPacket.rumbleData[4] = highFreq & 0xFF;
        _lastSwitchPacket.rumbleData[1] = _lastSwitchPacket.rumbleData[5] = (highAmp << 1) + ((highFreq >> 8) & 0x1);
        _lastSwitchPacket.rumbleData[2] = _lastSwitchPacket.rumbleData[6] = lowFreq;
        _lastSwitchPacket.rumbleData[3] = _lastSwitchPacket.rumbleData[7] = lowAmp;
            
        
        _lastSwitchPacket.reportID = 0x10; // Rumble only
        _lastSwitchPacket.sequence++;
        _lastSwitchPacket.sequence &= 0xF;
        _lastSwitchPacket.command = 0; // LED
        [self sendReport:[NSData dataWithBytes:&_lastSwitchPacket length:sizeof(_lastSwitchPacket)]];
    }
    else {
        if (_rumbleElement.max == 1 && _rumbleElement.min == 0) {
            [_rumblePWMThreadLock lock];
            _rumblePWMRatio = amp;
            if (!_rumblePWMThreadRunning) { // PWM thread not running, start it.
                if (amp != 0) {
                    _rumblePWMRatio = amp;
                    _rumblePWMThreadRunning = true;
                    [self performSelectorInBackground:@selector(pwmThread) withObject:nil];
                }
            }
            [_rumblePWMThreadLock unlock];
        }
        else {
            [_rumbleElement setValue:amp * (_rumbleElement.max - _rumbleElement.min) + _rumbleElement.min];
        }
    }
}

- (bool)isConnected
{
    return _logicallyConnected && _physicallyConnected;
}

- (void)_forceStopPWMThread
{
    [_rumblePWMThreadLock lock];
    if (_rumblePWMThreadRunning) {
        _forceStopPWMThread = true;
    }
    [_rumblePWMThreadLock unlock];
}

+ (void)controllerAdded:(IOHIDDeviceRef) device
{
    NSString *name = (__bridge NSString *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
    NSDictionary *hacks = hacksByName[name];
    if (!hacks) {
        hacks = hacksByManufacturer[(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey))];
    }
    NSArray *filters = hacks[JOYReportIDFilters];
    JOYController *controller = nil;
    if (filters) {
        controller = [[JOYMultiplayerController alloc] initWithDevice:device
                                                      reportIDFilters:filters];
    }
    else {
        controller = [[JOYController alloc] initWithDevice:device];
    }
    
    if (hacks[JOYActivationReport]) {
        [controller sendReport:hacks[JOYActivationReport]];
    }
    [controllers setObject:controller forKey:[NSValue valueWithPointer:device]];


}

+ (void)controllerRemoved:(IOHIDDeviceRef) device
{
    [[controllers objectForKey:[NSValue valueWithPointer:device]] disconnected];
    [controllers removeObjectForKey:[NSValue valueWithPointer:device]];
}

+ (NSArray<JOYController *> *)allControllers
{
    return exposedControllers;
}

+ (void)load
{
#include "ControllerConfiguration.inc"
}

+(void)registerListener:(id<JOYListener>)listener
{
    [listeners addObject:listener];
}

+(void)unregisterListener:(id<JOYListener>)listener
{
    [listeners removeObject:listener];
}

+ (void)startOnRunLoop:(NSRunLoop *)runloop withOptions: (NSDictionary *)options
{
    axesEmulateButtons = [options[JOYAxesEmulateButtonsKey] boolValue];
    axes2DEmulateButtons = [options[JOYAxes2DEmulateButtonsKey] boolValue];
    hatsEmulateButtons = [options[JOYHatsEmulateButtonsKey] boolValue];
    
    controllers = [NSMutableDictionary dictionary];
    exposedControllers = [NSMutableArray array];
    NSArray *array = @[
        CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick),
        CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad),
        CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController),
        @{@kIOHIDDeviceUsagePageKey: @(kHIDPage_Game)},
    ];

    listeners = [NSMutableSet set];
    static IOHIDManagerRef manager = nil;
    if (manager) {
        CFRelease(manager); // Stop the previous session
    }
    manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    
    if (!manager) return;
    if (IOHIDManagerOpen(manager, kIOHIDOptionsTypeNone)) {
        CFRelease(manager);
        return;
    }
    
    IOHIDManagerSetDeviceMatchingMultiple(manager, (__bridge CFArrayRef)array);
    IOHIDManagerRegisterDeviceMatchingCallback(manager, HIDDeviceAdded, NULL);
    IOHIDManagerRegisterDeviceRemovalCallback(manager, HIDDeviceRemoved, NULL);
    IOHIDManagerScheduleWithRunLoop(manager, [runloop getCFRunLoop], kCFRunLoopDefaultMode);
}

- (void)dealloc
{
    if (_device) {
        CFRelease(_device);
        _device = NULL;
    }
}
@end
