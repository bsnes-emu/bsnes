#import "JOYAxes3D.h"
#import "JOYElement.h"

@interface JOYAxes3D()
@property unsigned rotation; // in 90 degrees units, clockwise
@end

@implementation JOYAxes3D
{
    JOYElement *_element1, *_element2, *_element3;
    double _state1, _state2, _state3;
    int32_t _minX, _minY, _minZ;
    int32_t _maxX, _maxY, _maxZ;
    double _gApproximation;
}

+ (NSString *)usageToString: (JOYAxes3DUsage) usage
{
    if (usage < JOYAxes3DUsageNonGenericMax) {
        return (NSString *[]) {
            @"None",
            @"Acceleretion",
            @"Orientation",
            @"Gyroscope",
        }[usage];
    }
    if (usage >= JOYAxes3DUsageGeneric0) {
        return [NSString stringWithFormat:@"Generic 3D Analog Control %d", usage - JOYAxes3DUsageGeneric0];
    }
    
    return [NSString stringWithFormat:@"Unknown Usage 3D Axes %d", usage];
}

- (NSString *)usageString
{
    return [self.class usageToString:_usage];
}

- (uint64_t)uniqueID
{
    return _element1.uniqueID | (uint64_t)self.combinedIndex << 32;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@: %p, %@ (%llx); State: (%.2f, %.2f, %.2f)>", self.className, self, self.usageString, self.uniqueID, _state1, _state2, _state3];
}

- (instancetype)initWithFirstElement:(JOYElement *)element1 secondElement:(JOYElement *)element2 thirdElement:(JOYElement *)element3
{
    self = [super init];
    if (!self) return self;
    
    _element1 = element1;
    _element2 = element2;
    _element3 = element3;

    _maxX = element1? element1.max :  1;
    _maxY = element2? element2.max :  1;
    _maxZ = element3? element3.max :  1;
    _minX = element1? element1.min : -1;
    _minY = element2? element2.min : -1;
    _minZ = element3? element3.min : -1;
    
    return self;
}

- (JOYPoint3D)rawValue
{
    return (JOYPoint3D){_state1, _state2, _state3};
}

- (JOYPoint3D)normalizedValue
{
    double distance = sqrt(_state1 * _state1 + _state2 * _state2 + _state3 * _state3);
    if (distance == 0) {
        distance = 1;
    }
    return (JOYPoint3D){_state1 / distance, _state2 / distance, _state3 / distance};
}

- (JOYPoint3D)gUnitsValue
{
    double distance = _gApproximation ?: 1;
    return (JOYPoint3D){_state1 / distance, _state2 / distance, _state3 / distance};
}

- (bool)updateState
{
    int32_t x = [_element1 value];
    int32_t y = [_element2 value];
    int32_t z = [_element3 value];

    if (x == 0 && y == 0 && z == 0) return false;
    
    double old1 = _state1, old2 = _state2, old3 = _state3;
    _state1 = (x - _minX) / (double)(_maxX - _minX) * 2 - 1;
    _state2 = (y - _minY) / (double)(_maxY - _minY) * 2 - 1;
    _state3 = (z - _minZ) / (double)(_maxZ - _minZ) * 2 - 1;
    
    double distance = sqrt(_state1 * _state1 + _state2 * _state2 + _state3 * _state3);
    if (_gApproximation == 0) {
        _gApproximation = distance;
    }
    else {
        _gApproximation = _gApproximation * 0.9999 + distance * 0.0001;
    }
    
    double temp = _state1;
    switch (_rotation & 3) {
        case 0: break;
        case 1:
            _state1 = -_state3;
            _state3 = temp;
            break;
        case 2:
            _state1 = -_state1;
            _state3 = -_state3;
            break;
        case 3:
            _state1 = _state3;
            _state3 = -temp;
            break;
    }

    return old1 != _state1 || old2 != _state2 || old3 != _state3;
}

@end
