#import "GBStatesViewController.h"
#import "GBSlotButton.h"
#import "GBROMManager.h"
#import "GBViewController.h"

@implementation GBStatesViewController

- (void)updateSlotView:(GBSlotButton *)view
{
    NSString *stateFile = [[GBROMManager sharedManager] stateFile:view.tag];
    if ([[NSFileManager defaultManager] fileExistsAtPath:stateFile]) {
        NSDate *date = [[[NSFileManager defaultManager] attributesOfItemAtPath:stateFile error:nil] fileModificationDate];
        if (@available(iOS 13.0, *)) {
            if ((uint64_t)(date.timeIntervalSince1970) == (uint64_t)([NSDate now].timeIntervalSince1970)) {
                view.slotSubtitleLabel.text = @"Just now";
            }
            else {
                NSRelativeDateTimeFormatter *formatter = [[NSRelativeDateTimeFormatter alloc] init];
                view.slotSubtitleLabel.text = [formatter localizedStringForDate:date relativeToDate:[NSDate now]];
            }
        }
        else {
            NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
            formatter.timeStyle = NSDateFormatterShortStyle;
            formatter.dateStyle = NSDateFormatterShortStyle;
            formatter.doesRelativeDateFormatting = true;
            view.slotSubtitleLabel.text = [formatter stringFromDate:date];
        }
        
        view.imageView.image = [UIImage imageWithContentsOfFile:[stateFile stringByAppendingPathExtension:@"png"]];
    }
    else {
        view.slotSubtitleLabel.text = @"Empty";
        view.imageView.image = nil;
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    UIVisualEffect *effect = [UIBlurEffect effectWithStyle:(UIBlurEffectStyle)UIBlurEffectStyleProminent];
    self.view = [[UIVisualEffectView alloc] initWithEffect:effect];
    self.view.bounds = CGRectMake(0, 0, 0x300, 0x300);
    UIView *root = ((UIVisualEffectView *)self.view).contentView;
    for (unsigned i = 0; i < 9; i++) {
        unsigned x = i % 3;
        unsigned y = i / 3;
        GBSlotButton *slotView = [GBSlotButton buttonWithLabelText:[NSString stringWithFormat:@"Slot %u", i + 1]];
        
        slotView.frame = CGRectMake(0x100 * x,
                                    0x100 * y,
                                    0x100,
                                    0x100);
        
        slotView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin |
                                    UIViewAutoresizingFlexibleWidth |
                                    UIViewAutoresizingFlexibleRightMargin |
                                    UIViewAutoresizingFlexibleTopMargin |
                                    UIViewAutoresizingFlexibleHeight |
                                    UIViewAutoresizingFlexibleBottomMargin;

        slotView.tag = i + 1;
        [self updateSlotView:slotView];
        [slotView addTarget:self
                     action:@selector(slotSelected:)
           forControlEvents:UIControlEventTouchUpInside];
        [root addSubview:slotView];
    }
    self.edgesForExtendedLayout = 0;
}

- (void)slotSelected:(GBSlotButton *)slot
{
    
    UIAlertController *controller = [UIAlertController alertControllerWithTitle:slot.label.text
                                                                        message:nil
                                                                 preferredStyle:UIAlertControllerStyleActionSheet];
    
    NSString *stateFile = [[GBROMManager sharedManager] stateFile:slot.tag];
    GBViewController *delegate = (typeof(delegate))[UIApplication sharedApplication].delegate;
    
    void (^saveState)(UIAlertAction *action) = ^(UIAlertAction *action) {
        [delegate saveStateToFile:stateFile];
        [self updateSlotView:slot];
        [self.presentingViewController dismissViewControllerAnimated:true completion:nil];
        slot.showingMenu = false;
    };
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:stateFile]) {
        saveState(nil);
        return;
    }

    [controller addAction:[UIAlertAction actionWithTitle:@"Save state"
                                                   style:UIAlertActionStyleDefault
                                                 handler:saveState]];
    
    [controller addAction:[UIAlertAction actionWithTitle:@"Load state"
                                                   style:UIAlertActionStyleDefault
                                                 handler:^(UIAlertAction *action) {
        [delegate loadStateFromFile:stateFile];
        [self updateSlotView:slot];
        [self.presentingViewController dismissViewControllerAnimated:true completion:nil];
        slot.showingMenu = false;
    }]];
    
    [controller addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                   style:UIAlertActionStyleCancel
                                                 handler:^(UIAlertAction *action) {
        slot.showingMenu = false;
    }]];
        
    slot.showingMenu = true;
    controller.popoverPresentationController.sourceView = slot;
    [self presentViewController:controller animated:true completion:nil];
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    if ([UIDevice currentDevice].userInterfaceIdiom != UIUserInterfaceIdiomPad) {
        UIView *root = ((UIVisualEffectView *)self.view).contentView;
        CGRect frame = root.frame;
        UIEdgeInsets insets = [UIApplication sharedApplication].keyWindow.safeAreaInsets;
        frame.size.height = self.view.frame.size.height - insets.bottom;
        frame.size.width = self.view.frame.size.width -  insets.left - insets.right;
        frame.origin.x = insets.left;
        root.frame = frame;
    }
}

- (NSString *)title
{
    return @"Save States";
}
@end
