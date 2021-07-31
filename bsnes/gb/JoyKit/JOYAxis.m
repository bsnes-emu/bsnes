#import "JOYAxis.h"
#import "JOYElement.h"

@implementation JOYAxis
{
    JOYElement *_element;
    double _state;
    double _min;
}

+ (NSString *)usageToString: (JOYAxisUsage) usage
{
    if (usage < JOYAxisUsageNonGenericMax) {
        return (NSString *[]) {
            @"None",
            @"Analog L1",
            @"Analog L2",
            @"Analog L3",
            @"Analog R1",
            @"Analog R2",
            @"Analog R3",
            @"Slider",
            @"Dial",
            @"Wheel",
            @"Rudder",
            @"Throttle",
            @"Accelerator",
            @"Brake",
        }[usage];
    }
    if (usage >= JOYAxisUsageGeneric0) {
        return [NSString stringWithFormat:@"Generic Analog Control %d", usage - JOYAxisUsageGeneric0];
    }
    
    return [NSString stringWithFormat:@"Unknown Usage Axis %d", usage];
}

- (NSString *)usageString
{
    return [self.class usageToString:_usage];
}

- (uint64_t)uniqueID
{
    return _element.uniqueID;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@: %p, %@ (%llu); State: %f%%>", self.className, self, self.usageString, self.uniqueID, _state * 100];
}

- (instancetype)initWithElement:(JOYElement *)element
{
    self = [super init];
    if (!self) return self;
    
    _element = element;

    
    if (element.usagePage == kHIDPage_GenericDesktop) {
        switch (element.usage) {
            case kHIDUsage_GD_Slider: _usage = JOYAxisUsageSlider; break;
            case kHIDUsage_GD_Dial: _usage = JOYAxisUsageDial; break;
            case kHIDUsage_GD_Wheel: _usage = JOYAxisUsageWheel; break;
            default:
                _usage = JOYAxisUsageGeneric0 + element.usage - kHIDUsage_GD_X + 1;
                break;
        }
    }
    else if (element.usagePage == kHIDPage_Simulation) {
        switch (element.usage) {
            case kHIDUsage_Sim_Accelerator: _usage = JOYAxisUsageAccelerator; break;
            case kHIDUsage_Sim_Brake: _usage = JOYAxisUsageBrake; break;
            case kHIDUsage_Sim_Rudder: _usage = JOYAxisUsageRudder; break;
            case kHIDUsage_Sim_Throttle: _usage = JOYAxisUsageThrottle; break;
        }
    }
    _min = 1.0;
    
    return self;
}

- (double) value
{
    return _state;
}

- (bool)updateState
{
    double min = _element.min;
    double max = _element.max;
    if (min == max) return false;
    double old = _state;
    double unnormalized = ([_element value] - min) / (max - min);
    if (unnormalized < _min) {
        _min = unnormalized;
    }
    if (_min != 1) {
        _state = (unnormalized - _min) / (1 - _min);
    }
    return old != _state;
}

- (JOYButtonUsage)equivalentButtonUsage
{
    if (self.usage >= JOYAxisUsageGeneric0) {
        return self.usage - JOYAxisUsageGeneric0 + JOYButtonUsageGeneric0;
    }
    switch (self.usage) {
        case JOYAxisUsageL1: return JOYButtonUsageL1;
        case JOYAxisUsageL2: return JOYButtonUsageL2;
        case JOYAxisUsageL3: return JOYButtonUsageL3;
        case JOYAxisUsageR1: return JOYButtonUsageR1;
        case JOYAxisUsageR2: return JOYButtonUsageR2;
        case JOYAxisUsageR3: return JOYButtonUsageR3;
        case JOYAxisUsageSlider: return JOYButtonUsageSlider;
        case JOYAxisUsageDial: return JOYButtonUsageDial;
        case JOYAxisUsageWheel: return JOYButtonUsageWheel;
        case JOYAxisUsageRudder: return JOYButtonUsageRudder;
        case JOYAxisUsageThrottle: return JOYButtonUsageThrottle;
        case JOYAxisUsageAccelerator: return JOYButtonUsageAccelerator;
        case JOYAxisUsageBrake: return JOYButtonUsageBrake;
        default: return JOYButtonUsageNone;
    }
}


@end
