#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <JoyKit/JoyKit.h>

@interface GBJoyConManager : NSObject<JOYListener, NSTableViewDataSource, NSTableViewDelegate>
+ (instancetype) sharedInstance;

@property (nonatomic) bool arrangementMode;
@property (weak) IBOutlet NSTableView *tableView;
@property (nonatomic) IBOutlet NSButton *autoPairCheckbox;
@property (nonatomic) IBOutlet NSButton *horizontalCheckbox;
@end

