#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <UserNotifications/UserNotifications.h>

typedef enum {
    GBRunModeNormal,
    GBRunModeTurbo,
    GBRunModeRewind,
    GBRunModePaused,
    GBRunModeUnderclock,
} GBRunMode;

@interface GBViewController : UIViewController <UIApplicationDelegate,
                                                AVCaptureVideoDataOutputSampleBufferDelegate,
                                                UNUserNotificationCenterDelegate>
@property (nonatomic, strong) UIWindow *window;
- (void)reset;
- (void)openLibrary;
- (void)start;
- (void)stop;
- (void)changeModel;
- (void)openStates;
- (void)openSettings;
- (void)showAbout;
- (void)openConnectMenu;
- (void)openCheats;
- (void)emptyPrinterFeed;
- (void)saveStateToFile:(NSString *)file;
- (bool)loadStateFromFile:(NSString *)file;
- (bool)handleOpenURLs:(NSArray <NSURL *> *)urls
           openInPlace:(bool)inPlace;
- (void)dismissViewController;
@property (nonatomic) GBRunMode runMode;
@end
