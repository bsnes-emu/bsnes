#import <Foundation/Foundation.h>

@interface JOYInput : NSObject
@property (readonly) unsigned combinedIndex;
- (NSString *)usageString;
- (uint64_t)uniqueID;
@end

