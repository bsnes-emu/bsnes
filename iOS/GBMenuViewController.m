#import <objc/runtime.h>
#import "GBMenuViewController.h"
#import "GBMenuButton.h"
#import "GBViewController.h"
#import "GBROMManager.h"

@interface GBMenuViewController ()

@end

@implementation GBMenuViewController

+ (instancetype)menu
{
    UIAlertControllerStyle style = [UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad?
                                       UIAlertControllerStyleAlert : UIAlertControllerStyleActionSheet;
    GBMenuViewController *ret = [self alertControllerWithTitle:nil
                                                       message:nil
                                                preferredStyle:style];
    [ret addAction:[UIAlertAction actionWithTitle:@"Close"
                                            style:UIAlertActionStyleCancel
                                          handler:^(UIAlertAction * _Nonnull action) {
        [(GBViewController *)[UIApplication sharedApplication].delegate start];
    }]];
    return ret;
}

// The redundant sizeof forces the compiler to validate the selector exists
#define SelectorString(x) (sizeof(@selector(x))? @#x : nil)

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:true];
    static const struct {
        NSString *label;
        NSString *image;
        NSString *selector;
    } buttons[] = {
        {@"Reset", @"arrow.2.circlepath", SelectorString(reset)},
        {@"Library", @"bookmark", SelectorString(openLibrary)},
        {@"Model", @"ModelTemplate", nil},
        {@"States", @"square.stack", nil},
        {@"Settings", @"gear", nil},
        {@"About", @"info.circle", nil},
    };
    
    double width = self.view.frame.size.width / 3;
    double height = 88;
    for (unsigned i = 0; i < 6; i++) {
        unsigned x = i % 3;
        unsigned y = i / 3;
        GBMenuButton *button = [GBMenuButton buttonWithType:UIButtonTypeSystem];
        [button setTitle:buttons[i].label forState:UIControlStateNormal];
        if (@available(iOS 13.0, *)) {
            UIImage *image = [UIImage imageNamed:buttons[i].image] ?: [UIImage systemImageNamed:buttons[i].image];
            [button setImage:image forState:UIControlStateNormal];
        }
        button.frame = CGRectMake(width * x, height * y, width, height);
        button.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin;
        [self.view addSubview:button];

        if (!buttons[i].selector) {
            button.enabled = false;
            continue;
        }
        SEL selector = NSSelectorFromString(buttons[i].selector);
        if (selector == @selector(reset) && ![GBROMManager sharedManager].currentROM) {
            button.enabled = false;
            continue;
        }
        id block = ^(){
            [self.presentingViewController dismissViewControllerAnimated:true completion:^{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
                (void)[[UIApplication sharedApplication].delegate performSelector:selector];
#pragma clang diagnostic pop
            }];
        };
        objc_setAssociatedObject(button, "RetainedBlock", block, OBJC_ASSOCIATION_RETAIN);
        [button addTarget:block action:@selector(invoke) forControlEvents:UIControlEventTouchUpInside];
        

    }
}

- (void)viewDidLayoutSubviews
{
    if (self.view.bounds.size.height < 88 * 2) {
        [self.view.heightAnchor constraintEqualToConstant:self.view.bounds.size.height + 88 * 2].active = true;
    }
    [super viewDidLayoutSubviews];
}

// This is a hack that forces the iPad controller to display the button separator
- (UIViewController *)contentViewController
{
    return [[UIViewController alloc] init];
}

@end
