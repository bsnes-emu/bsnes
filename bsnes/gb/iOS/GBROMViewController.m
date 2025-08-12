#import "GBROMViewController.h"
#import "GBROMManager.h"
#import "GBViewController.h"
#import "GBLibraryViewController.h"
#import <CoreServices/CoreServices.h>
#import <objc/runtime.h>

@implementation GBROMViewController
{
    NSIndexPath *_renamingPath;
    NSArray *_roms;
    __weak UIAlertController *_alertToRemove;
}

- (instancetype)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(reactivate)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 1) return 2;
    return (_roms = [GBROMManager sharedManager].allROMs).count;
}

- (UITableViewCell *)cellForROM:(NSString *)rom
{
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    cell.textLabel.text = rom.lastPathComponent;
    bool isCurrentROM = [rom isEqualToString:[GBROMManager sharedManager].currentROM];
    bool checkmark = isCurrentROM;
    cell.accessoryType = checkmark? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    
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
        [[UIColor colorWithWhite:0 alpha:0.5] set];
    }
    [mask stroke];
    cell.imageView.image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return cell;

}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) {
        UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
        switch (indexPath.item) {
            case 0: cell.textLabel.text = @"Import ROM files"; break;
            case 1: cell.textLabel.text = @"Show Library in Files"; break;
        }
        return cell;
    }
    return [self cellForROM:_roms[[indexPath indexAtPosition:1]]];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) return [super tableView:tableView heightForRowAtIndexPath:indexPath];

    return 60;
}

- (NSString *)title
{
    return @"Local Library";
}

- (NSString *)tableView:(UITableView *)tableView titleForFooterInSection:(NSInteger)section
{
    if (section == 0) return nil;

    return @"You can also import ROM files by opening them in SameBoy using the Files app or a web browser, or by sending them over with AirDrop.";
}

- (void)romSelectedAtIndex:(unsigned)index
{
    NSString *rom = _roms[index];
    [GBROMManager sharedManager].currentROM = rom;
    [self.presentingViewController dismissViewControllerAnimated:true completion:nil];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) {
        switch (indexPath.item) {
            case 0: {
                UIViewController *parent = self.presentingViewController;
                NSString *gbUTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)@"gb", NULL);
                NSString *gbcUTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)@"gbc", NULL);
                NSString *isxUTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)@"isx", NULL);
                NSString *zipUTI = (__bridge_transfer NSString *)UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, (__bridge CFStringRef)@"zip", NULL);

                NSMutableSet *extensions = [NSMutableSet set];
                [extensions addObjectsFromArray:(__bridge_transfer NSArray *)UTTypeCopyAllTagsWithClass((__bridge CFStringRef)gbUTI, kUTTagClassFilenameExtension)];
                [extensions addObjectsFromArray:(__bridge_transfer NSArray *)UTTypeCopyAllTagsWithClass((__bridge CFStringRef)gbcUTI, kUTTagClassFilenameExtension)];
                [extensions addObjectsFromArray:(__bridge_transfer NSArray *)UTTypeCopyAllTagsWithClass((__bridge CFStringRef)isxUTI, kUTTagClassFilenameExtension)];
                
                if (extensions.count != 3) {
                    if (![[NSUserDefaults standardUserDefaults] boolForKey:@"GBShownUTIWarning"]) {
                        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"File Association Conflict"
                                                                                       message:@"Due to a limitation in iOS, the file picker will allow you to select files not supported by SameBoy. SameBoy will only import GB, GBC and ISX files.\n\nIf you have a multi-system emulator installed, updating it could fix this problem."
                                                                                preferredStyle:UIAlertControllerStyleAlert];
                        [alert addAction:[UIAlertAction actionWithTitle:@"Close"
                                                                  style:UIAlertActionStyleCancel
                                                                handler:^(UIAlertAction *action) {
                            [[NSUserDefaults standardUserDefaults] setBool:true forKey:@"GBShownUTIWarning"];
                            [self tableView:tableView didSelectRowAtIndexPath:indexPath];
                        }]];
                        [self presentViewController:alert animated:true completion:nil];
                        return;
                    }
                }
                                
                [self.presentingViewController dismissViewControllerAnimated:true completion:^{
                    UIDocumentPickerViewController *picker = [[UIDocumentPickerViewController alloc] initWithDocumentTypes:@[@"com.github.liji32.sameboy.gb",
                                                                                                                             @"com.github.liji32.sameboy.gbc",
                                                                                                                             @"com.github.liji32.sameboy.isx",
                                                                                                                             gbUTI ?: @"",
                                                                                                                             gbcUTI ?: @"",
                                                                                                                             isxUTI ?: @"",
                                                                                                                             zipUTI ?: @""]
                                                                                                                    inMode:UIDocumentPickerModeImport];
                    picker.allowsMultipleSelection = true;
                    if (@available(iOS 13.0, *)) {
                        picker.shouldShowFileExtensions = true;
                    }
                    picker.delegate = self;
                    objc_setAssociatedObject(picker, @selector(delegate), self, OBJC_ASSOCIATION_RETAIN);
                    [parent presentViewController:picker animated:true completion:nil];
                }];
                return;
            }
            case 1: {
                NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"shareddocuments://%@",
                                                   [self.rootPath stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLPathAllowedCharacterSet]]]];
                [[UIApplication sharedApplication] openURL:url
                                                   options:nil
                                         completionHandler:nil];
                return;
            }
        }
    }
    [self romSelectedAtIndex:indexPath.row];
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray <NSURL *>*)urls
{
    [(GBViewController *)[UIApplication sharedApplication].delegate handleOpenURLs:urls
                                                                       openInPlace:false];
}

- (UIModalPresentationStyle)modalPresentationStyle
{
    return UIModalPresentationOverFullScreen;
}

- (void)deleteROMAtIndex:(unsigned)index
{
    NSString *rom = _roms[index];

    [[GBROMManager sharedManager] deleteROM:rom];
    [self.tableView deleteRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:index inSection:0]] withRowAnimation:UITableViewRowAnimationAutomatic];
    if ([[GBROMManager sharedManager].currentROM isEqualToString:rom]) {
        [GBROMManager sharedManager].currentROM = nil;
    }
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) return;

    if (editingStyle != UITableViewCellEditingStyleDelete) return;
    NSString *rom = [self.tableView cellForRowAtIndexPath:indexPath].textLabel.text;
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"Delete “%@”?", rom]
                                                                   message: @"Save data for this ROM will also be deleted."
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"Delete"
                                              style:UIAlertActionStyleDestructive
                                            handler:^(UIAlertAction *action) {
        [self deleteROMAtIndex:indexPath.row];
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                              style:UIAlertActionStyleCancel
                                            handler:nil]];
    _alertToRemove = alert; // indexPath becoomes invalid if we reload, dismiss the alert if it happens
    [self presentViewController:alert animated:true completion:nil];
}

- (void)renameRow:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) return;
    
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    CGRect frame = cell.textLabel.frame;
    frame.size.width = cell.textLabel.superview.frame.size.width - 8 - frame.origin.x;
    UITextField *field = [[UITextField alloc] initWithFrame:frame];
    field.font = cell.textLabel.font;
    field.text = cell.textLabel.text;
    cell.textLabel.textColor = [UIColor clearColor];
    [[cell.textLabel superview] addSubview:field];
    [field becomeFirstResponder];
    [field selectAll:nil];
    _renamingPath = indexPath;
    [field addTarget:self action:@selector(doneRename:) forControlEvents:UIControlEventEditingDidEnd | UIControlEventEditingDidEndOnExit];
}

- (void)renameROM:(NSString *)oldName toName:(NSString *)newName
{
    [[GBROMManager sharedManager] renameROM:oldName toName:newName];
    [self.tableView reloadData];
}

- (void)doneRename:(UITextField *)sender
{
    if (!_renamingPath) return;
    NSString *newName = sender.text;
    NSString *oldName = [self.tableView cellForRowAtIndexPath:_renamingPath].textLabel.text;
    
    _renamingPath = nil;
    if ([newName isEqualToString:oldName]) {
        [self.tableView reloadData];
        return;
    }
    
    if ([newName containsString:@"/"]) {
        [self.tableView reloadData];
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Invalid Name"
                                                                       message:@"You can't use a name that contains “/”. Please choose another name."
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"OK"
                                                  style:UIAlertActionStyleCancel
                                                handler:nil]];
        [self presentViewController:alert animated:true completion:nil];
        return;
    }
    if ([[GBROMManager sharedManager].forbiddenNames containsObject:newName]) {
        [self.tableView reloadData];
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Invalid Name"
                                                                       message:@"This name is reserved by SameBoy or iOS. Please choose another name."
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"OK"
                                                  style:UIAlertActionStyleCancel
                                                handler:nil]];
        [self presentViewController:alert animated:true completion:nil];
        return;
    }
    [self renameROM:oldName toName:newName];
    _renamingPath = nil;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return indexPath.section == 0;
}

- (void)duplicateROMAtIndex:(unsigned)index
{
    [[GBROMManager sharedManager] duplicateROM:_roms[index]];
    [self.tableView reloadData];
}


// Leave these ROM management to iOS 13.0 and up for now
- (UIContextMenuConfiguration *)tableView:(UITableView *)tableView
contextMenuConfigurationForRowAtIndexPath:(NSIndexPath *)indexPath
                                    point:(CGPoint)point API_AVAILABLE(ios(13.0))
{
    if (indexPath.section == 1) return nil;
    
    return [UIContextMenuConfiguration configurationWithIdentifier:nil
                                                   previewProvider:nil
                                                    actionProvider:^UIMenu *(NSArray<UIMenuElement *> *suggestedActions) {
        UIAction *deleteAction = [UIAction actionWithTitle:@"Delete"
                                                     image:[UIImage systemImageNamed:@"trash"]
                                                identifier:nil
                                                   handler:^(UIAction *action) {
            [self tableView:tableView
         commitEditingStyle:UITableViewCellEditingStyleDelete
          forRowAtIndexPath:indexPath];
        }];
        deleteAction.attributes = UIMenuElementAttributesDestructive;
        NSMutableArray *items = @[
            [UIAction actionWithTitle:@"Rename"
                                image:[UIImage systemImageNamed:@"pencil"]
                           identifier:nil
                              handler:^(__kindof UIAction *action) {
                [self renameRow:indexPath];
            }],
            [UIAction actionWithTitle:@"Duplicate"
                                image:[UIImage systemImageNamed:@"plus.square.on.square"]
                           identifier:nil
                              handler:^(__kindof UIAction *action) {
                [self duplicateROMAtIndex:indexPath.row];
            }],
        ].mutableCopy;
        [items addObject:deleteAction];
        return [UIMenu menuWithTitle:nil children:items];
    }];
}

- (void)deselectRow
{
    if (self.tableView.indexPathForSelectedRow) {
        [self.tableView deselectRowAtIndexPath:self.tableView.indexPathForSelectedRow animated:true];
    }
}

- (void)reactivate
{
    [self deselectRow];
    // Do not auto-reload if busy
    if (self.view.window.userInteractionEnabled) {
        [self.tableView reloadData];
        if (self.presentedViewController == _alertToRemove) {
            [self dismissViewControllerAnimated:true completion:nil];
        }
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [self.tableView reloadData];
    [super viewWillAppear:animated];
}

- (NSString *)rootPath
{
    return [GBROMManager sharedManager].localRoot;
}

@end
