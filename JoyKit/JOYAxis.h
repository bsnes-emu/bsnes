#import <Foundation/Foundation.h>
#import "JOYButton.h"

typedef enum {
    JOYAxisUsageNone,
    JOYAxisUsageL1,
    JOYAxisUsageL2,
    JOYAxisUsageL3,
    JOYAxisUsageR1,
    JOYAxisUsageR2,
    JOYAxisUsageR3,
    
    JOYAxisUsageSlider,
    JOYAxisUsageDial,
    JOYAxisUsageWheel,
    
    JOYAxisUsageRudder,
    JOYAxisUsageThrottle,
    JOYAxisUsageAccelerator,
    JOYAxisUsageBrake,
    
    JOYAxisUsageNonGenericMax,
    
    JOYAxisUsageGeneric0 = 0x10000,
} JOYAxisUsage;

@interface JOYAxis : NSObject
- (NSString *)usageString;
+ (NSString *)usageToString: (JOYAxisUsage) usage;
- (uint64_t)uniqueID;
- (double)value;
- (JOYButtonUsage)equivalentButtonUsage;
@property JOYAxisUsage usage;
@end


