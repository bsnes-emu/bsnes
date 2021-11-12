#import <Foundation/Foundation.h>

typedef enum {
    JOYButtonUsageNone,
    JOYButtonUsageA,
    JOYButtonUsageB,
    JOYButtonUsageC,
    JOYButtonUsageX,
    JOYButtonUsageY,
    JOYButtonUsageZ,
    JOYButtonUsageStart,
    JOYButtonUsageSelect,
    JOYButtonUsageHome,
    JOYButtonUsageMisc,
    JOYButtonUsageLStick,
    JOYButtonUsageRStick,
    JOYButtonUsageL1,
    JOYButtonUsageL2,
    JOYButtonUsageL3,
    JOYButtonUsageR1,
    JOYButtonUsageR2,
    JOYButtonUsageR3,
    JOYButtonUsageDPadLeft,
    JOYButtonUsageDPadRight,
    JOYButtonUsageDPadUp,
    JOYButtonUsageDPadDown,
    
    JOYButtonUsageSlider,
    JOYButtonUsageDial,
    JOYButtonUsageWheel,
    
    JOYButtonUsageRudder,
    JOYButtonUsageThrottle,
    JOYButtonUsageAccelerator,
    JOYButtonUsageBrake,
    
    JOYButtonUsageNonGenericMax,
    
    JOYButtonUsageGeneric0 = 0x10000,
} JOYButtonUsage;

typedef enum {
    JOYButtonTypeNormal,
    JOYButtonTypeAxisEmulated,
    JOYButtonTypeAxes2DEmulated,
    JOYButtonTypeHatEmulated,
} JOYButtonType;

@interface JOYButton : NSObject
- (NSString *)usageString;
+ (NSString *)usageToString: (JOYButtonUsage) usage;
- (uint64_t)uniqueID;
- (bool) isPressed;
@property JOYButtonUsage usage;
@property (readonly) JOYButtonType type;
@end


