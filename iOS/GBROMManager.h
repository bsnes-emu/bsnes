#import <Foundation/Foundation.h>

@interface GBROMManager : NSObject
+ (instancetype) sharedManager;

@property (readonly) NSArray<NSString *> *allROMs;
@property (nonatomic) NSString *currentROM;
@property (readonly) NSString *romFile;
@end
