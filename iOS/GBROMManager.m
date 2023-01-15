#import "GBROMManager.h"

@implementation GBROMManager
{
    NSString *_romFile;
}

+ (instancetype)sharedManager
{
    static dispatch_once_t onceToken;
    static GBROMManager *manager;
    dispatch_once(&onceToken, ^{
        manager = [[self alloc] init];
    });
    return manager;
}

- (instancetype)init
{
    self = [super init];
    if (!self) return nil;
    self.currentROM = [[NSUserDefaults standardUserDefaults] stringForKey:@"GBLastROM"];
    return self;
}

- (void)setCurrentROM:(NSString *)currentROM
{
    _romFile = nil;
    _currentROM = currentROM;
    if (currentROM && !self.romFile) {
        _currentROM = nil;
    }
    
    [[NSUserDefaults standardUserDefaults] setObject:_currentROM forKey:@"GBLastROM"];
}

- (NSString *)romFileForDirectory:(NSString *)romDirectory
{
    for (NSString *filename in [NSFileManager.defaultManager enumeratorAtPath:romDirectory]) {
        if ([@[@"gb", @"gbc", @"isx"] containsObject:filename.pathExtension.lowercaseString]) {
            return [romDirectory stringByAppendingPathComponent:filename];
        }
    }
    
    return nil;
}

- (NSString *)romFile
{
    if (_romFile) return _romFile;
    if (!_currentROM) return nil;
    NSString *root = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true).firstObject;
    NSString *romDirectory = [root stringByAppendingPathComponent:_currentROM];
    return _romFile = [self romFileForDirectory:romDirectory];
}

- (NSString *)romFileForROM:(NSString *)rom
{
    if (rom == _currentROM) {
        return self.romFile;
    }
    NSString *root = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true).firstObject;
    NSString *romDirectory = [root stringByAppendingPathComponent:rom];
    return [self romFileForDirectory:romDirectory];
}

- (NSString *)auxilaryFileForROM:(NSString *)rom withExtension:(NSString *)extension
{
    return [[[self romFileForROM:rom] stringByDeletingPathExtension] stringByAppendingPathExtension:extension];
}

- (NSString *)batterySaveFileForROM:(NSString *)rom
{
    return [self auxilaryFileForROM:rom withExtension:@"sav"];
}

- (NSString *)batterySaveFile
{
    return [self batterySaveFileForROM:_currentROM];
}

- (NSString *)autosaveStateFileForROM:(NSString *)rom
{
    return [self auxilaryFileForROM:rom withExtension:@"auto"];
}

- (NSString *)autosaveStateFile
{
    return [self autosaveStateFileForROM:_currentROM];
}

- (NSString *)stateFile:(unsigned)index forROM:(NSString *)rom
{
    return [self auxilaryFileForROM:rom withExtension:[NSString stringWithFormat:@"s%u", index]];
}

- (NSString *)stateFile:(unsigned)index
{
    return [self stateFile:index forROM:_currentROM];
}

- (NSArray<NSString *> *)allROMs
{
    NSMutableArray<NSString *> *ret = [NSMutableArray array];
    NSString *root = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true).firstObject;
    for (NSString *romDirectory in [NSFileManager.defaultManager enumeratorAtPath:root]) {
        if ([self romFileForDirectory:[root stringByAppendingPathComponent:romDirectory]]) {
            [ret addObject:romDirectory];
        }
    }
    return ret;
}

@end
