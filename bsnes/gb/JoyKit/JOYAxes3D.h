#import <Foundation/Foundation.h>
#import "JOYInput.h"

typedef enum {
    JOYAxes3DUsageNone,
    JOYAxes3DUsageAcceleration,
    JOYAxes3DUsageOrientation,
    JOYAxes3DUsageGyroscope,
    JOYAxes3DUsageNonGenericMax,
    
    JOYAxes3DUsageGeneric0 = 0x10000,
} JOYAxes3DUsage;

typedef struct {
    double x, y, z;
} JOYPoint3D;

@interface JOYAxes3D : JOYInput
+ (NSString *)usageToString: (JOYAxes3DUsage) usage;
- (JOYPoint3D)rawValue;
- (JOYPoint3D)normalizedValue; // For orientation
- (JOYPoint3D)gUnitsValue; // For acceleration
@property JOYAxes3DUsage usage;
@end


