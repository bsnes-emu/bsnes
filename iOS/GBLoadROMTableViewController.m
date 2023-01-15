#import "GBLoadROMTableViewController.h"
#import "GBTableViewCell.h"
#import "GBROMManager.h"

@interface GBLoadROMTableViewController ()

@end

@implementation GBLoadROMTableViewController

- (instancetype)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
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
    GBTableViewCell *cell = [[GBTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    NSString *rom = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    cell.textLabel.text = rom;
    cell.accessoryType = [rom isEqualToString:[GBROMManager sharedManager].currentROM]? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    
    NSString *pngPath = [[[GBROMManager sharedManager] autosaveStateFileForROM:rom] stringByAppendingPathExtension:@"png"];
    UIImage *image = [UIImage imageWithContentsOfFile:pngPath];
    if (!image) {
        static dispatch_once_t onceToken;
        static UIImage *emptyImage = nil;
        dispatch_once(&onceToken, ^{
            UIGraphicsBeginImageContextWithOptions((CGSize){160, 144}, false, 1);
            emptyImage = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
        });
        image = emptyImage;
    }
    cell.padding = 4;
    cell.imageView.image = image;
    cell.imageView.contentMode = UIViewContentModeScaleAspectFit;
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 64;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return @"ROM Library";
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

@end
