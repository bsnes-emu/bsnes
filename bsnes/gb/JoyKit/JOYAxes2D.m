#import "JOYAxes2D.h"
#import "JOYElement.h"

@interface JOYAxes2D()
@property unsigned rotation; // in 90 degrees units, clockwise
@end

@implementation JOYAxes2D
{
    JOYElement *_element1, *_element2;
    double _state1, _state2;
    int32_t _initialX, _initialY;
    int32_t _minX, _minY;
    int32_t _maxX, _maxY;
}

+ (NSString *)usageToString: (JOYAxes2DUsage) usage
{
    if (usage < JOYAxes2DUsageNonGenericMax) {
        return inline_const(NSString *[], {
            @"None",
            @"Left Stick",
            @"Right Stick",
            @"Middle Stick",
            @"Pointer",
        })[usage];
    }
    if (usage >= JOYAxes2DUsageGeneric0) {
        return [NSString stringWithFormat:@"Generic 2D Analog Control %d", usage - JOYAxes2DUsageGeneric0];
    }
    
    return [NSString stringWithFormat:@"Unknown Usage 2D Axes %d", usage];
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
    return [NSString stringWithFormat:@"<%@: %p, %@ (%llx); State: %.2f%%, %.2f degrees>", self.className, self, self.usageString, self.uniqueID, self.distance * 100, self.angle];
}

- (instancetype)initWithFirstElement:(JOYElement *)element1 secondElement:(JOYElement *)element2
{
    self = [super init];
    if (!self) return self;
    
    _element1 = element1;
    _element2 = element2;

    
    if (element1.usagePage == kHIDPage_GenericDesktop) {
        uint16_t usage = element1.usage;
        _usage = JOYAxes2DUsageGeneric0 + usage - kHIDUsage_GD_X + 1;
    }
    _initialX = 0;
    _initialY = 0;
    _minX = element1.max;
    _minY = element2.max;
    _maxX = element1.min;
    _maxY = element2.min;
    
    return self;
}

- (NSPoint)value
{
    return NSMakePoint(_state1, _state2);
}

- (int32_t)effectiveMinX
{
    int32_t rawMin = _element1.min;
    int32_t rawMax = _element1.max;
    if (_initialX == 0) return rawMin;
    if (_minX <= (rawMin * 2 + _initialX) / 3 && _maxX >= (rawMax * 2 + _initialX) / 3 ) return _minX;
    if ((_initialX - rawMin) < (rawMax - _initialX)) return rawMin;
    return _initialX - (rawMax - _initialX);
}

- (int32_t)effectiveMinY
{
    int32_t rawMin = _element2.min;
    int32_t rawMax = _element2.max;
    if (_initialY == 0) return rawMin;
    if (_minX <= (rawMin * 2 + _initialY) / 3 && _maxY >= (rawMax * 2 + _initialY) / 3 ) return _minY;
    if ((_initialY - rawMin) < (rawMax - _initialY)) return rawMin;
    return _initialY - (rawMax - _initialY);
}

- (int32_t)effectiveMaxX
{
    int32_t rawMin = _element1.min;
    int32_t rawMax = _element1.max;
    if (_initialX == 0) return rawMax;
    if (_minX <= (rawMin * 2 + _initialX) / 3 && _maxX >= (rawMax * 2 + _initialX) / 3 ) return _maxX;
    if ((_initialX - rawMin) > (rawMax - _initialX)) return rawMax;
    return _initialX + (_initialX - rawMin);
}

- (int32_t)effectiveMaxY
{
    int32_t rawMin = _element2.min;
    int32_t rawMax = _element2.max;
    if (_initialY == 0) return rawMax;
    if (_minX <= (rawMin * 2 + _initialY) / 3 && _maxY >= (rawMax * 2 + _initialY) / 3 ) return _maxY;
    if ((_initialY - rawMin) > (rawMax - _initialY)) return rawMax;
    return _initialY + (_initialY - rawMin);
}

- (bool)updateState
{
    int32_t x = [_element1 value];
    int32_t y = [_element2 value];
    if (x == 0 && y == 0) return false;
    
    if (_initialX == 0 && _initialY == 0) {
         _initialX = x;
         _initialY = y;
    }
    
    double old1 = _state1, old2 = _state2;
    {
        int32_t value = x;

        if (_initialX != 0) {
            _minX = MIN(value, _minX);
            _maxX = MAX(value, _maxX);
        }
        
        double min = [self effectiveMinX];
        double max = [self effectiveMaxX];
        if (min == max) return false;
        
        _state1 = (value - min) / (max - min) * 2 - 1;
    }
    
    {
        int32_t value = y;

        if (_initialY != 0) {
            _minY = MIN(value, _minY);
            _maxY = MAX(value, _maxY);
        }
        
        double min = [self effectiveMinY];
        double max = [self effectiveMaxY];
        if (min == max) return false;
        
        _state2 = (value - min) / (max - min) * 2 - 1;
    }
    
    if (_state1 < -1 || _state1 > 1 ||
        _state2 < -1 || _state2 > 1) {
        // Makes no sense, recalibrate
        _state1 = _state2 = 0;
        _initialX = _initialY = 0;
        _minX = _element1.max;
        _minY = _element2.max;
        _maxX = _element1.min;
        _maxY = _element2.min;
    }
    
    
    double temp = _state1;
    switch (_rotation & 3) {
        case 0: break;
        case 1:
            _state1 = -_state2;
            _state2 = temp;
            break;
        case 2:
            _state1 = -_state1;
            _state2 = -_state2;
            break;
        case 3:
            _state1 = _state2;
            _state2 = -temp;
            break;
    }

    return old1 != _state1 || old2 != _state2;
}

- (double)distance
{
    return MIN(sqrt(_state1 * _state1 + _state2 * _state2), 1.0);
}

- (double)angle
{
    double temp = atan2(_state2, _state1) * 180 / M_PI;
    if (temp >= 0) return temp;
    return temp + 360;
}

@end
