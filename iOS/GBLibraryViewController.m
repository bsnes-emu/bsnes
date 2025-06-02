#import "GBLibraryViewController.h"
#import "GBROMViewController.h"
#import "GBViewController.h"
#import "GBROMManager.h"


@implementation GBLibraryViewController

+ (UIViewController *)wrapViewController:(UIViewController *)controller
{
    UINavigationController *ret = [[UINavigationController alloc] initWithRootViewController:controller];
    UIBarButtonItem *close = [[UIBarButtonItem alloc] initWithTitle:@"Close"
                                                              style:UIBarButtonItemStylePlain
                                                             target:[UIApplication sharedApplication].delegate
                                                             action:@selector(dismissViewController)];
    [ret.visibleViewController.navigationItem setLeftBarButtonItem:close];
    return ret;
}


- (instancetype)init
{
    return (GBLibraryViewController *)[self.class wrapViewController:[[GBROMViewController alloc] init]];
}

@end
