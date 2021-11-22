#import "JOYController.h"
#import "JOYMultiplayerController.h"
#import "JOYElement.h"
#import "JOYSubElement.h"
#import "JOYFullReportElement.h"
#import "JOYButton.h"
#import "JOYEmulatedButton.h"
#include <IOKit/hid/IOHIDLib.h>

#include <AppKit/AppKit.h>
extern NSTextField *globalDebugField;

#define PWM_RESOLUTION 16

static NSString const *JOYAxisGroups = @"JOYAxisGroups";
static NSString const *JOYReportIDFilters = @"JOYReportIDFilters";
static NSString const *JOYButtonUsageMapping = @"JOYButtonUsageMapping";
static NSString const *JOYAxisUsageMapping = @"JOYAxisUsageMapping";
static NSString const *JOYAxes2DUsageMapping = @"JOYAxes2DUsageMapping";
static NSString const *JOYCustomReports = @"JOYCustomReports";
static NSString const *JOYIsSwitch = @"JOYIsSwitch";
static NSString const *JOYRumbleUsage = @"JOYRumbleUsage";
static NSString const *JOYRumbleUsagePage = @"JOYRumbleUsagePage";
static NSString const *JOYConnectedUsage = @"JOYConnectedUsage";
static NSString const *JOYConnectedUsagePage = @"JOYConnectedUsagePage";
static NSString const *JOYRumbleMin = @"JOYRumbleMin";
static NSString const *JOYRumbleMax = @"JOYRumbleMax";
static NSString const *JOYSwapZRz = @"JOYSwapZRz";
static NSString const *JOYActivationReport = @"JOYActivationReport";
static NSString const *JOYIgnoredReports = @"JOYIgnoredReports";
static NSString const *JOYIsDualShock3 = @"JOYIsDualShock3";
static NSString const *JOYIsSony = @"JOYIsSony";
static NSString const *JOYEmulateAxisButtons = @"JOYEmulateAxisButtons";

static NSMutableDictionary<id, JOYController *> *controllers; // Physical controllers
static NSMutableArray<JOYController *> *exposedControllers; // Logical controllers

static NSDictionary *hacksByName = nil;
static NSDictionary *hacksByManufacturer = nil;

static NSMutableSet<id<JOYListener>> *listeners = nil;

static bool axes2DEmulateButtons = false;
static bool hatsEmulateButtons = false;

@interface JOYController ()
+ (void)controllerAdded:(IOHIDDeviceRef) device;
+ (void)controllerRemoved:(IOHIDDeviceRef) device;
- (void)elementChanged:(IOHIDElementRef) element;
- (void)gotReport:(NSData *)report;

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

@interface JOYAxes3D ()
{
    @public JOYElement *_element1, *_element2, *_element3;
}
- (instancetype)initWithFirstElement:(JOYElement *)element1 secondElement:(JOYElement *)element2 thirdElement:(JOYElement *)element2;
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

static void HIDReport(void *context, IOReturn result, void *sender, IOHIDReportType type,
                      uint32_t reportID, uint8_t *report, CFIndex reportLength)
{
    if (reportLength) {
        [(__bridge JOYController *)context gotReport:[[NSData alloc] initWithBytesNoCopy:report length:reportLength freeWhenDone:false]];
    }
}

typedef struct __attribute__((packed)) {
    uint8_t reportID;
    uint8_t sequence;
    uint8_t rumbleData[8];
    uint8_t command;
    uint8_t commandData[26];
} JOYSwitchPacket;

typedef struct __attribute__((packed)) {
    uint8_t reportID;
    uint8_t padding;
    uint8_t rumbleRightDuration;
    uint8_t rumbleRightStrength;
    uint8_t rumbleLeftDuration;
    uint8_t rumbleLeftStrength;
    uint32_t padding2;
    uint8_t ledsEnabled;
    struct {
        uint8_t timeEnabled;
        uint8_t dutyLength;
        uint8_t enabled;
        uint8_t dutyOff;
        uint8_t dutyOn;
    } __attribute__((packed)) led[5];
    uint8_t padding3[13];
} JOYDualShock3Output;

typedef struct __attribute__((packed)) {
    uint8_t reportID;
    uint8_t sequence;
    union {
        uint8_t tag;
        uint8_t reportIDOnUSB;
    };
    uint16_t flags;
    uint8_t rumbleRightStrength; // Weak
    uint8_t rumbleLeftStrength; // Strong
    uint8_t reserved[4];
    uint8_t muteButtonLED;
    uint8_t powerSaveControl;
    uint8_t reserved2[28];
    uint8_t flags2;
    uint8_t reserved3[2];
    uint8_t lightbarSetup;
    uint8_t LEDBrightness;
    uint8_t playerLEDs;
    uint8_t lightbarRed;
    uint8_t lightbarGreen;
    uint8_t lightbarBlue;
    uint8_t bluetoothSpecific[24];
    uint32_t crc32;
} JOYDualSenseOutput;


typedef union {
    JOYSwitchPacket switchPacket;
    JOYDualShock3Output ds3Output;
    JOYDualSenseOutput dualsenseOutput;
} JOYVendorSpecificOutput;

@implementation JOYController
{
    IOHIDDeviceRef _device;
    NSMutableDictionary<JOYElement *, JOYButton *> *_buttons;
    NSMutableDictionary<JOYElement *, JOYAxis *> *_axes;
    NSMutableDictionary<JOYElement *, JOYAxes2D *> *_axes2D;
    NSMutableDictionary<JOYElement *, JOYAxes3D *> *_axes3D;
    NSMutableDictionary<JOYElement *, JOYHat *> *_hats;
    NSMutableDictionary<NSNumber *, JOYFullReportElement *> *_fullReportElements;
    NSMutableDictionary<JOYFullReportElement *, NSArray<JOYElement *> *> *_multiElements;
    JOYAxes3D *_lastAxes3D;
    
    // Button emulation
    NSMutableDictionary<NSNumber *, JOYEmulatedButton *> *_axisEmulatedButtons;
    NSMutableDictionary<NSNumber *, NSArray <JOYEmulatedButton *> *> *_axes2DEmulatedButtons;
    NSMutableDictionary<NSNumber *, NSArray <JOYEmulatedButton *> *> *_hatEmulatedButtons;

    JOYElement *_rumbleElement;
    JOYElement *_connectedElement;
    NSMutableDictionary<NSValue *, JOYElement *> *_iokitToJOY;
    NSString *_serialSuffix;
    bool _isSwitch; // Does this controller use the Switch protocol?
    bool _isDualShock3; // Does this controller use DS3 outputs?
    bool _isSony; // Is this a DS4 or newer Sony controller?
    bool _isDualSense;
    bool _isUSBDualSense;

    JOYVendorSpecificOutput _lastVendorSpecificOutput;
    volatile double _rumbleAmplitude;
    bool _physicallyConnected;
    bool _logicallyConnected;
    
    NSDictionary *_hacks;
    NSMutableData *_lastReport;
    
    // Used when creating inputs
    JOYElement *_previousAxisElement;
    
    uint8_t _playerLEDs;
    double _sentRumbleAmp;
    unsigned _rumbleCounter;
    bool _deviceCantSendReports;
    dispatch_queue_t _rumbleQueue;
}

- (instancetype)initWithDevice:(IOHIDDeviceRef) device hacks:(NSDictionary *)hacks
{
    return [self initWithDevice:device reportIDFilter:nil serialSuffix:nil hacks:hacks];
}

-(void)createOutputForElement:(JOYElement *)element
{
    uint16_t rumbleUsagePage = (uint16_t)[_hacks[JOYRumbleUsagePage] unsignedIntValue];
    uint16_t rumbleUsage = (uint16_t)[_hacks[JOYRumbleUsage] unsignedIntValue];

    if (!_rumbleElement && rumbleUsage && rumbleUsagePage && element.usage == rumbleUsage && element.usagePage == rumbleUsagePage) {
        if (_hacks[JOYRumbleMin]) {
            element.min = [_hacks[JOYRumbleMin] unsignedIntValue];
        }
        if (_hacks[JOYRumbleMax]) {
            element.max = [_hacks[JOYRumbleMax] unsignedIntValue];
        }
        _rumbleElement = element;
    }
}

-(void)createInputForElement:(JOYElement *)element
{
    uint16_t connectedUsagePage = (uint16_t)[_hacks[JOYConnectedUsagePage] unsignedIntValue];
    uint16_t connectedUsage = (uint16_t)[_hacks[JOYConnectedUsage] unsignedIntValue];

    if (!_connectedElement && connectedUsage && connectedUsagePage && element.usage == connectedUsage && element.usagePage == connectedUsagePage) {
        _connectedElement = element;
        _logicallyConnected = element.value != element.min;
        return;
    }
    
    NSDictionary *axisGroups = @{
        @(kHIDUsage_GD_X): @(0),
        @(kHIDUsage_GD_Y): @(0),
        @(kHIDUsage_GD_Z): @(1),
        @(kHIDUsage_GD_Rx): @(2),
        @(kHIDUsage_GD_Ry): @(2),
        @(kHIDUsage_GD_Rz): @(1),
    };
    
    if (element.usagePage == kHIDPage_Sensor) {
        JOYAxes3DUsage usage;
        JOYElement *element1 = nil, *element2 = nil, *element3 = nil;
        
        switch (element.usage) {
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisX:
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisY:
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisZ:
                usage = JOYAxes3DUsageAcceleration;
                break;
            case kHIDUsage_Snsr_Data_Motion_AngularPositionXAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularPositionYAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularPositionZAxis:
                usage = JOYAxes3DUsageOrientation;
                break;
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityXAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityYAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityZAxis:
                usage = JOYAxes3DUsageGyroscope;
                break;
            default:
                return;
        }
        
        switch (element.usage) {
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisX:
            case kHIDUsage_Snsr_Data_Motion_AngularPositionXAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityXAxis:
                element1 = element;
                if (_lastAxes3D && !_lastAxes3D->_element1 && _lastAxes3D.usage == usage) {
                    element2 = _lastAxes3D->_element2;
                    element3 = _lastAxes3D->_element3;
                }
                break;
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisY:
            case kHIDUsage_Snsr_Data_Motion_AngularPositionYAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityYAxis:
                element2 = element;
                if (_lastAxes3D && !_lastAxes3D->_element2 && _lastAxes3D.usage == usage) {
                    element1 = _lastAxes3D->_element1;
                    element3 = _lastAxes3D->_element3;
                }
                break;
            case kHIDUsage_Snsr_Data_Motion_AccelerationAxisZ:
            case kHIDUsage_Snsr_Data_Motion_AngularPositionZAxis:
            case kHIDUsage_Snsr_Data_Motion_AngularVelocityZAxis:
                element3 = element;
                if (_lastAxes3D && !_lastAxes3D->_element3 && _lastAxes3D.usage == usage) {
                    element1 = _lastAxes3D->_element1;
                    element2 = _lastAxes3D->_element2;
                }
                break;
        }
        
        _lastAxes3D = [[JOYAxes3D alloc] initWithFirstElement:element1 secondElement:element2 thirdElement:element3];
        _lastAxes3D.usage = usage;
        if (element1) _axes3D[element1] = _lastAxes3D;
        if (element2) _axes3D[element2] = _lastAxes3D;
        if (element3) _axes3D[element3] = _lastAxes3D;
        
        return;
    }
    
    axisGroups = _hacks[JOYAxisGroups] ?: axisGroups;
    
    if (element.usagePage == kHIDPage_Button ||
        (element.usagePage == kHIDPage_Consumer && (element.usage == kHIDUsage_Csmr_ACHome ||
                                                    element.usage == kHIDUsage_Csmr_ACBack))) {
    button: {
        JOYButton *button = [[JOYButton alloc] initWithElement: element];
        [_buttons setObject:button forKey:element];
        NSNumber *replacementUsage = element.usagePage == kHIDPage_Button? _hacks[JOYButtonUsageMapping][@(button.usage)] : nil;
        if (replacementUsage) {
            button.usage = [replacementUsage unsignedIntValue];
        }
        return;
    }
    }
    else if (element.usagePage == kHIDPage_Simulation) {
        switch (element.usage) {
            case kHIDUsage_Sim_Accelerator:
            case kHIDUsage_Sim_Brake:
            case kHIDUsage_Sim_Rudder:
            case kHIDUsage_Sim_Throttle:
            goto single;
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
                
                JOYElement *other = _previousAxisElement;
                _previousAxisElement = element;
                if (!other) goto single;
                if (other.usage >= element.usage) goto single;
                if (other.reportID != element.reportID) goto single;
                if (![axisGroups[@(other.usage)] isEqualTo: axisGroups[@(element.usage)]]) goto single;
                if (other.parentID != element.parentID) goto single;
                
                JOYAxes2D *axes = nil;
                if (other.usage == kHIDUsage_GD_Z && element.usage == kHIDUsage_GD_Rz && [_hacks[JOYSwapZRz] boolValue]) {
                    axes = [[JOYAxes2D alloc] initWithFirstElement:element secondElement:other];
                }
                else {
                    axes = [[JOYAxes2D alloc] initWithFirstElement:other secondElement:element];
                }
                NSNumber *replacementUsage = _hacks[JOYAxes2DUsageMapping][@(axes.usage)];
                if (replacementUsage) {
                    axes.usage = [replacementUsage unsignedIntValue];
                }
                
                [_axisEmulatedButtons removeObjectForKey:@(_axes[other].uniqueID)];
                [_axes removeObjectForKey:other];
                _previousAxisElement = nil;
                _axes2D[other] = axes;
                _axes2D[element] = axes;
                
                if (axes2DEmulateButtons) {
                    _axes2DEmulatedButtons[@(axes.uniqueID)] = @[
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadLeft type:JOYButtonTypeAxes2DEmulated uniqueID:axes.uniqueID | 0x100000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadRight type:JOYButtonTypeAxes2DEmulated uniqueID:axes.uniqueID | 0x200000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadUp type:JOYButtonTypeAxes2DEmulated  uniqueID:axes.uniqueID | 0x300000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadDown type:JOYButtonTypeAxes2DEmulated uniqueID:axes.uniqueID | 0x400000000L],
                    ];
                }
                break;
            }
            case kHIDUsage_GD_Slider:
            case kHIDUsage_GD_Dial:
            case kHIDUsage_GD_Wheel:
            { single: {
                JOYAxis *axis = [[JOYAxis alloc] initWithElement: element];
                [_axes setObject:axis forKey:element];
                
                NSNumber *replacementUsage = element.usagePage == kHIDPage_GenericDesktop? _hacks[JOYAxisUsageMapping][@(axis.usage)] : nil;
                if (replacementUsage) {
                    axis.usage = [replacementUsage unsignedIntValue];
                }
                
                if ([_hacks[JOYEmulateAxisButtons] boolValue]) {
                    _axisEmulatedButtons[@(axis.uniqueID)] =
                    [[JOYEmulatedButton alloc] initWithUsage:axis.equivalentButtonUsage type:JOYButtonTypeAxisEmulated uniqueID:axis.uniqueID];
                }
                
                
                break;
            }}
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
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadLeft type:JOYButtonTypeHatEmulated  uniqueID:hat.uniqueID | 0x100000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadRight type:JOYButtonTypeHatEmulated uniqueID:hat.uniqueID | 0x200000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadUp type:JOYButtonTypeHatEmulated  uniqueID:hat.uniqueID | 0x300000000L],
                        [[JOYEmulatedButton alloc] initWithUsage:JOYButtonUsageDPadDown type:JOYButtonTypeHatEmulated uniqueID:hat.uniqueID | 0x400000000L],
                    ];
                }
                break;
            }
        }
    }
}

- (instancetype)initWithDevice:(IOHIDDeviceRef)device reportIDFilter:(NSArray <NSNumber *> *) filter serialSuffix:(NSString *)suffix hacks:(NSDictionary *)hacks
{
    self = [super init];
    if (!self) return self;
    
    _physicallyConnected = true;
    _logicallyConnected = true;
    _device = (IOHIDDeviceRef)CFRetain(device);
    _serialSuffix = suffix;
    _playerLEDs = -1;

    IOHIDDeviceRegisterInputValueCallback(device, HIDInput, (void *)self);
    IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);

    NSArray *array = CFBridgingRelease(IOHIDDeviceCopyMatchingElements(device, NULL, kIOHIDOptionsTypeNone));
    _buttons = [NSMutableDictionary dictionary];
    _axes = [NSMutableDictionary dictionary];
    _axes2D = [NSMutableDictionary dictionary];
    _axes3D = [NSMutableDictionary dictionary];
    _hats = [NSMutableDictionary dictionary];
    _axisEmulatedButtons = [NSMutableDictionary dictionary];
    _axes2DEmulatedButtons = [NSMutableDictionary dictionary];
    _hatEmulatedButtons = [NSMutableDictionary dictionary];
    _iokitToJOY = [NSMutableDictionary dictionary];
    
    
    _hacks = hacks;
    _isSwitch = [_hacks[JOYIsSwitch] boolValue];
    _isDualShock3 = [_hacks[JOYIsDualShock3] boolValue];
    _isSony = [_hacks[JOYIsSony] boolValue];

    NSDictionary *customReports = hacks[JOYCustomReports];
    _lastReport = [NSMutableData dataWithLength:MAX(
                                                    MAX(
                                                        [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDMaxInputReportSizeKey)) unsignedIntValue],
                                                        [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDMaxOutputReportSizeKey)) unsignedIntValue]
                                                        ),
                                                    [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDMaxFeatureReportSizeKey)) unsignedIntValue]
                                                    )];
    IOHIDDeviceRegisterInputReportCallback(device, _lastReport.mutableBytes, _lastReport.length, HIDReport, (void *)self);

    if (hacks[JOYCustomReports]) {
        _multiElements = [NSMutableDictionary dictionary];
        _fullReportElements = [NSMutableDictionary dictionary];

        
        for (NSNumber *_reportID in customReports) {
            signed reportID = [_reportID intValue];
            bool isOutput = false;
            if (reportID < 0) {
                isOutput = true;
                reportID = -reportID;
            }
            
            JOYFullReportElement *element = [[JOYFullReportElement alloc] initWithDevice:device reportID:reportID];
            NSMutableArray *elements = [NSMutableArray array];
            for (NSDictionary <NSString *,NSNumber *> *subElementDef in customReports[_reportID]) {
                if (filter && subElementDef[@"reportID"] && ![filter containsObject:subElementDef[@"reportID"]]) continue;
                JOYSubElement *subElement = [[JOYSubElement alloc] initWithRealElement:element
                                                                                  size:subElementDef[@"size"].unsignedLongValue
                                                                                offset:subElementDef[@"offset"].unsignedLongValue + 8 // Compensate for the reportID
                                                                             usagePage:subElementDef[@"usagePage"].unsignedLongValue
                                                                                 usage:subElementDef[@"usage"].unsignedLongValue
                                                                                   min:subElementDef[@"min"].unsignedIntValue
                                                                                   max:subElementDef[@"max"].unsignedIntValue];
                [elements addObject:subElement];
                if (isOutput) {
                    [self createOutputForElement:subElement];
                }
                else {
                    [self createInputForElement:subElement];
                }
            }
            _multiElements[element] = elements;
            if (!isOutput) {
                _fullReportElements[@(reportID)] = element;
            }
        }
    }
    
    id previous = nil;
    NSSet *ignoredReports = nil;
    if (hacks[JOYIgnoredReports]) {
        ignoredReports = [NSSet setWithArray:hacks[JOYIgnoredReports]];
    }
    
    for (id _element in array) {
        if (_element == previous) continue; // Some elements are reported twice for some reason
        previous = _element;
        JOYElement *element = [[JOYElement alloc] initWithElement:(__bridge IOHIDElementRef)_element];

        bool isOutput = false;
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
        if ((!isOutput && [ignoredReports containsObject:@(element.reportID)]) ||
            (isOutput && [ignoredReports containsObject:@(-element.reportID)])) continue;

        
        if (IOHIDElementIsArray((__bridge IOHIDElementRef)_element)) continue;
        
        if (isOutput) {
            [self createOutputForElement:element];
        }
        else {
            [self createInputForElement:element];
        }
        
        _iokitToJOY[@(IOHIDElementGetCookie((__bridge IOHIDElementRef)_element))] = element;
    }
    
    [exposedControllers addObject:self];
    if (_logicallyConnected) {
        for (id<JOYListener> listener in listeners) {
            if ([listener respondsToSelector:@selector(controllerConnected:)]) {
                [listener controllerConnected:self];
            }
        }
    }
    
    if (_hacks[JOYActivationReport]) {
        [self sendReport:hacks[JOYActivationReport]];
    }
    
    if (_isSwitch) {
        [self sendReport:[NSData dataWithBytes:(uint8_t[]){0x80, 0x04} length:2]];
        [self sendReport:[NSData dataWithBytes:(uint8_t[]){0x80, 0x02} length:2]];
        
        _lastVendorSpecificOutput.switchPacket.reportID = 0x1; // Rumble and LEDs
        _lastVendorSpecificOutput.switchPacket.sequence++;
        _lastVendorSpecificOutput.switchPacket.sequence &= 0xF;
        _lastVendorSpecificOutput.switchPacket.command = 3; // Set input report mode
        _lastVendorSpecificOutput.switchPacket.commandData[0] = 0x30; // Standard full mode
        [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.switchPacket length:sizeof(_lastVendorSpecificOutput.switchPacket)]];
        
        _lastVendorSpecificOutput.switchPacket.sequence++;
        _lastVendorSpecificOutput.switchPacket.sequence &= 0xF;
        _lastVendorSpecificOutput.switchPacket.command = 0x40; // Enable/disableIMU
        _lastVendorSpecificOutput.switchPacket.commandData[0] = 1; // Enabled
        [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.switchPacket length:sizeof(_lastVendorSpecificOutput.switchPacket)]];
    }
    
    if (_isDualShock3) {
        _lastVendorSpecificOutput.ds3Output = (JOYDualShock3Output){
            .reportID = 1,
            .led = {
                {.timeEnabled =  0xff, .dutyLength = 0x27, .enabled = 0x10, .dutyOff = 0, .dutyOn = 0x32},
                {.timeEnabled =  0xff, .dutyLength = 0x27, .enabled = 0x10, .dutyOff = 0, .dutyOn = 0x32},
                {.timeEnabled =  0xff, .dutyLength = 0x27, .enabled = 0x10, .dutyOff = 0, .dutyOn = 0x32},
                {.timeEnabled =  0xff, .dutyLength = 0x27, .enabled = 0x10, .dutyOff = 0, .dutyOn = 0x32},
                {.timeEnabled =  0,    .dutyLength = 0,    .enabled = 0,    .dutyOff = 0, .dutyOn = 0},
            }
        };
    }
    if (_isSony) {
        _isDualSense = [(__bridge NSNumber *)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey)) unsignedIntValue] == 0xce6;
    }
    
    if (_isDualSense) {
        _isUSBDualSense = [(__bridge NSString *)IOHIDDeviceGetProperty(_device, CFSTR(kIOHIDTransportKey)) isEqualToString:@"USB"];
        _lastVendorSpecificOutput.dualsenseOutput = (JOYDualSenseOutput){
            .reportID = 0x31,
            .tag = 0x10,
            .flags = 0x1403, // Rumble, lightbar and player LEDs
            .flags2 = 2,
            .lightbarSetup = 2,
            .lightbarBlue = 255,
        };
        if (_isUSBDualSense) {
            _lastVendorSpecificOutput.dualsenseOutput.reportIDOnUSB = 1;
            _lastVendorSpecificOutput.dualsenseOutput.lightbarBlue = 0;
            _lastVendorSpecificOutput.dualsenseOutput.lightbarGreen = 96;
            _lastVendorSpecificOutput.dualsenseOutput.lightbarRed = 255;

        }
        // Send a report to switch the controller to a more capable mode
        [self sendDualSenseOutput];
        _lastVendorSpecificOutput.dualsenseOutput.flags2 = 0;
        _lastVendorSpecificOutput.dualsenseOutput.lightbarSetup = 0;
    }
    
    _rumbleQueue = dispatch_queue_create([NSString stringWithFormat:@"Rumble Queue for %@", self.deviceName].UTF8String,
                                         NULL);
    
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

- (NSArray<JOYAxes3D *> *)axes3D
{
    return [[NSSet setWithArray:[_axes3D allValues]] allObjects];
}

- (NSArray<JOYHat *> *)hats
{
    return [_hats allValues];
}

- (void)gotReport:(NSData *)report
{
    JOYFullReportElement *element = _fullReportElements[@(*(uint8_t *)report.bytes)];
    if (element) {
        [element updateValue:report];
        
        NSArray<JOYElement *> *subElements = _multiElements[element];
        if (subElements) {
            for (JOYElement *subElement in subElements) {
                [self _elementChanged:subElement];
            }
        }
    }
    dispatch_async(_rumbleQueue, ^{
        [self updateRumble];
    });
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
        JOYAxes3D *axes = _axes3D[element];
        if (axes) {
            if ([axes updateState]) {
                for (id<JOYListener> listener in listeners) {
                    if ([listener respondsToSelector:@selector(controller:movedAxes3D:)]) {
                        [listener controller:self movedAxes3D:axes];
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
    [exposedControllers removeObject:self];
    [self setRumbleAmplitude:0];
    dispatch_sync(_rumbleQueue, ^{
        [self updateRumble];
    });
    _device = nil;
}

- (void)sendReport:(NSData *)report
{
    if (!report.length) return;
    if (!_device) return;
    if (_deviceCantSendReports) return;
    /* Some Macs fail to send reports to some devices, specifically the DS3, returning the bogus(?) error code 1 after
       freezing for 5 seconds. Stop sending reports if that's the case. */
    if (IOHIDDeviceSetReport(_device, kIOHIDReportTypeOutput, *(uint8_t *)report.bytes, report.bytes, report.length) == 1) {
        _deviceCantSendReports = true;
        NSLog(@"This Mac appears to be incapable of sending output reports to %@", self);
    }
}

- (void) sendDualSenseOutput
{
    if (_isUSBDualSense) {
        [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.dualsenseOutput.reportIDOnUSB length:_lastVendorSpecificOutput.dualsenseOutput.bluetoothSpecific - &_lastVendorSpecificOutput.dualsenseOutput.reportIDOnUSB]];
        return;
    }
    _lastVendorSpecificOutput.dualsenseOutput.sequence += 0x10;
    static const uint32_t table[] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };
    
    const uint8_t *byte = (void *)&_lastVendorSpecificOutput.dualsenseOutput;
    uint32_t size = sizeof(_lastVendorSpecificOutput.dualsenseOutput) - 4;
    uint32_t ret = 0xFFFFFFFF;
    ret = table[(ret ^ 0xa2) & 0xFF] ^ (ret >> 8);

    while (size--) {
        ret = table[(ret ^ *byte++) & 0xFF] ^ (ret >> 8);
    }
    
    _lastVendorSpecificOutput.dualsenseOutput.crc32 = ~ret;
    
    [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.dualsenseOutput length:sizeof(_lastVendorSpecificOutput.dualsenseOutput)]];
}

- (uint8_t)LEDMaskForPlayer:(unsigned)player
{
    if (_isDualShock3) {
        return 2 << player;
    }
    if (_isDualSense) {
        switch (player) {
            case 0: return 0x04;
            case 1: return 0x0A;
            case 2: return 0x15;
            case 3: return 0x1B;
            default: return 0;
        }
    }
    return 1 << player;
}

- (void)setPlayerLEDs:(uint8_t)mask
{
    if (mask == _playerLEDs) {
        return;
    }
    _playerLEDs = mask;
    if (_isSwitch) {
        _lastVendorSpecificOutput.switchPacket.reportID = 0x1; // Rumble and LEDs
        _lastVendorSpecificOutput.switchPacket.sequence++;
        _lastVendorSpecificOutput.switchPacket.sequence &= 0xF;
        _lastVendorSpecificOutput.switchPacket.command = 0x30; // LED
        _lastVendorSpecificOutput.switchPacket.commandData[0] = mask & 0xF;
        [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.switchPacket length:sizeof(_lastVendorSpecificOutput.switchPacket)]];
    }
    else if (_isDualShock3) {
        _lastVendorSpecificOutput.ds3Output.reportID = 1;
        _lastVendorSpecificOutput.ds3Output.ledsEnabled = (mask  & 0x1F);
        [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.ds3Output length:sizeof(_lastVendorSpecificOutput.ds3Output)]];
    }
    else if (_isDualSense) {
        _lastVendorSpecificOutput.dualsenseOutput.playerLEDs = mask & 0x1F;
        [self sendDualSenseOutput];
    }
}

- (void)updateRumble
{
    if (!self.connected) {
        return;
    }
    if (!_rumbleElement && !_isSwitch && !_isDualShock3 && !_isDualSense) {
        return;
    }
    if (_rumbleElement.max == 1 && _rumbleElement.min == 0) {
        double ampToSend = _rumbleCounter < round(_rumbleAmplitude * PWM_RESOLUTION);
        if (ampToSend != _sentRumbleAmp) {
            [_rumbleElement setValue:ampToSend];
            _sentRumbleAmp = ampToSend;
        }
        _rumbleCounter += round(_rumbleAmplitude * PWM_RESOLUTION);
        if (_rumbleCounter >= PWM_RESOLUTION) {
            _rumbleCounter -= PWM_RESOLUTION;
        }
    }
    else {
        if (_rumbleAmplitude == _sentRumbleAmp) {
            return;
        }
        _sentRumbleAmp = _rumbleAmplitude;
        if (_isSwitch) {
            double frequency = 144;
            double amp = _rumbleAmplitude;
            
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
            
            _lastVendorSpecificOutput.switchPacket.rumbleData[0] = _lastVendorSpecificOutput.switchPacket.rumbleData[4] = highFreq & 0xFF;
            _lastVendorSpecificOutput.switchPacket.rumbleData[1] = _lastVendorSpecificOutput.switchPacket.rumbleData[5] = (highAmp << 1) + ((highFreq >> 8) & 0x1);
            _lastVendorSpecificOutput.switchPacket.rumbleData[2] = _lastVendorSpecificOutput.switchPacket.rumbleData[6] = lowFreq;
            _lastVendorSpecificOutput.switchPacket.rumbleData[3] = _lastVendorSpecificOutput.switchPacket.rumbleData[7] = lowAmp;
            
            
            _lastVendorSpecificOutput.switchPacket.reportID = 0x10; // Rumble only
            _lastVendorSpecificOutput.switchPacket.sequence++;
            _lastVendorSpecificOutput.switchPacket.sequence &= 0xF;
            _lastVendorSpecificOutput.switchPacket.command = 0; // LED
            [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.switchPacket length:sizeof(_lastVendorSpecificOutput.switchPacket)]];
        }
        else if (_isDualShock3) {
            _lastVendorSpecificOutput.ds3Output.reportID = 1;
            _lastVendorSpecificOutput.ds3Output.rumbleLeftDuration = _lastVendorSpecificOutput.ds3Output.rumbleRightDuration = _rumbleAmplitude? 0xff : 0;
            _lastVendorSpecificOutput.ds3Output.rumbleLeftStrength = _lastVendorSpecificOutput.ds3Output.rumbleRightStrength = round(_rumbleAmplitude * 0xff);
            [self sendReport:[NSData dataWithBytes:&_lastVendorSpecificOutput.ds3Output length:sizeof(_lastVendorSpecificOutput.ds3Output)]];
        }
        else if (_isDualSense) {
            _lastVendorSpecificOutput.dualsenseOutput.rumbleLeftStrength = round(_rumbleAmplitude * _rumbleAmplitude * 0xff);
            _lastVendorSpecificOutput.dualsenseOutput.rumbleRightStrength = _rumbleAmplitude > 0.25 ? round(pow(_rumbleAmplitude - 0.25, 2) * 0xff) : 0;
            [self sendDualSenseOutput];
        }
        else {
            [_rumbleElement setValue:_rumbleAmplitude * (_rumbleElement.max - _rumbleElement.min) + _rumbleElement.min];
        }
    }
}

- (void)setRumbleAmplitude:(double)amp /* andFrequency: (double)frequency */
{
    if (amp < 0) amp = 0;
    if (amp > 1) amp = 1;
    _rumbleAmplitude = amp;
}

- (bool)isConnected
{
    return _logicallyConnected && _physicallyConnected;
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
                                                      reportIDFilters:filters
                                                                hacks:hacks];
    }
    else {
        controller = [[JOYController alloc] initWithDevice:device hacks:hacks];
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
