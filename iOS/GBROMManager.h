#import <Foundation/Foundation.h>

@interface GBROMManager : NSObject
+ (instancetype) sharedManager;

@property (readonly) NSArray<NSString *> *allROMs;
@property (nonatomic) NSString *currentROM;

@property (readonly) NSString *romFile;
@property (readonly) NSString *batterySaveFile;
@property (readonly) NSString *autosaveStateFile;
- (NSString *)stateFile:(unsigned)index;

- (NSString *)romFileForROM:(NSString *)rom;
- (NSString *)batterySaveFileForROM:(NSString *)rom;
- (NSString *)autosaveStateFileForROM:(NSString *)rom;
- (NSString *)stateFile:(unsigned)index forROM:(NSString *)rom;

@end
