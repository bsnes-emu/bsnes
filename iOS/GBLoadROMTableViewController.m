#import "GBLoadROMTableViewController.h"
#import "GBROMManager.h"

@interface GBLoadROMTableViewController ()

@end

@implementation GBLoadROMTableViewController

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
    cell.textLabel.text = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [GBROMManager sharedManager].currentROM = [GBROMManager sharedManager].allROMs[[indexPath indexAtPosition:1]];
    [self.presentingViewController dismissViewControllerAnimated:true completion:^{
        [[NSNotificationCenter defaultCenter] postNotificationName:@"GBROMChanged" object:nil];
    }];
}

- (BOOL)isModalInPresentation
{
    return true;
}

@end
