#import "GBLoadROMTableViewController.h"
#import "GBROMManager.h"

@interface GBLoadROMTableViewController ()

@end

@implementation GBLoadROMTableViewController

- (instancetype)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [GBROMManager sharedManager].allROMs.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    NSString *rom = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    cell.textLabel.text = rom;
    cell.accessoryType = [rom isEqualToString:[GBROMManager sharedManager].currentROM]? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    
    NSString *pngPath = [[[GBROMManager sharedManager] autosaveStateFileForROM:rom] stringByAppendingPathExtension:@"png"];
    UIGraphicsBeginImageContextWithOptions((CGSize){60, 60}, false, self.view.window.screen.scale);
    UIBezierPath *mask = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(0, 3, 60, 54) cornerRadius:4];
    [mask addClip];
    UIImage *image = [UIImage imageWithContentsOfFile:pngPath];
    [image drawInRect:mask.bounds];
    if (@available(iOS 13.0, *)) {
        [[UIColor tertiaryLabelColor] set];
    }
    else {
        [UIColor colorWithWhite:0 alpha:0.5];
    }
    [mask stroke];
    cell.imageView.image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60;
}

- (NSString *)title
{
    return @"ROM Library";
}

- (NSString *)tableView:(UITableView *)tableView titleForFooterInSection:(NSInteger)section
{
    return @"Import ROMs by opening them in SameBoy using the Files app or a web browser, or by sending them over with AirDrop";
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [GBROMManager sharedManager].currentROM = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    [self.presentingViewController dismissViewControllerAnimated:true completion:^{
        [[NSNotificationCenter defaultCenter] postNotificationName:@"GBROMChanged" object:nil];
    }];
}

- (UIModalPresentationStyle)modalPresentationStyle
{
    return UIModalPresentationFormSheet;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle != UITableViewCellEditingStyleDelete) return;
    NSString *rom = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"Delete ROM “%@”?", rom]
                                                                   message: @"Save data for this ROM will also be deleted."
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert  addAction:[UIAlertAction actionWithTitle:@"Delete"
                                               style:UIAlertActionStyleDestructive
                                             handler:^(UIAlertAction * _Nonnull action) {
        [[GBROMManager sharedManager] deleteROM:rom];
        [self.tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
        if ([[GBROMManager sharedManager].currentROM isEqualToString:rom]) {
            [GBROMManager sharedManager].currentROM = nil;
            [[NSNotificationCenter defaultCenter] postNotificationName:@"GBROMChanged" object:nil];
        }
    }]];
    [alert  addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                               style:UIAlertActionStyleCancel
                                             handler:nil]];
    [self presentViewController:alert animated:true completion:nil];
}

@end
