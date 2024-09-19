#import "GBPalettePicker.h"

@interface GBPalettePicker ()
{
    NSArray <NSString *>* _cacheNames;
    NSIndexPath *_renamingPath;
}

@end

@implementation GBPalettePicker

+ (const GB_palette_t *)paletteForTheme:(NSString *)theme
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if ([theme isEqualToString:@"Greyscale"]) {
        return &GB_PALETTE_GREY;
    }
    if ([theme isEqualToString:@"Lime (Game Boy)"]) {
        return &GB_PALETTE_DMG;
    }
    if ([theme isEqualToString:@"Olive (Pocket)"]) {
        return &GB_PALETTE_MGB;
    }
    if ([theme isEqualToString:@"Teal (Light)"]) {
        return &GB_PALETTE_GBL;
    }
    static GB_palette_t customPalette;
    NSArray *colors = [defaults dictionaryForKey:@"GBThemes"][theme][@"Colors"];
    if (colors.count != 5) return &GB_PALETTE_DMG;
    unsigned i = 0;
    for (NSNumber *color in colors) {
        uint32_t c = [color unsignedIntValue];
        customPalette.colors[i++] = (struct GB_color_s) {c, c >> 8, c >> 16};
    }
    return &customPalette;
}

+ (UIColor *)colorFromGBColor:(const struct GB_color_s *)color
{
    return [UIColor colorWithRed:color->r / 255.0
                           green:color->g / 255.0
                            blue:color->b / 255.0
                           alpha:1.0];
}

+ (UIImage *)previewImageForTheme:(NSString *)theme
{
    const GB_palette_t *palette = [self paletteForTheme:theme];
    UIGraphicsBeginImageContextWithOptions((CGSize){29, 29}, false, [UIScreen mainScreen].scale);
    UIBezierPath *path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(0, 0, 29, 29) cornerRadius:7];
    [[self colorFromGBColor:&palette->colors[4]] set];
    [path fill];
    
    path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(4, 4, 9, 9) cornerRadius:2];
    [[self colorFromGBColor:&palette->colors[0]] set];
    [path fill];
    
    path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(16, 4, 9, 9) cornerRadius:2];
    [[self colorFromGBColor:&palette->colors[1]] set];
    [path fill];
    
    path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(4, 16, 9, 9) cornerRadius:2];
    [[self colorFromGBColor:&palette->colors[2]] set];
    [path fill];
    
    path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(16, 16, 9, 9) cornerRadius:2];
    [[self colorFromGBColor:&palette->colors[3]] set];
    [path fill];
    
    UIImage *ret = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return ret;
}

- (instancetype)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
    return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 3;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0) return 4;
    if (section == 2) return 1;
    _cacheNames = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBThemes"].allKeys sortedArrayUsingSelector:@selector(localizedCaseInsensitiveCompare:)];
    return _cacheNames.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    
    NSString *name = nil;
    if (indexPath.section == 2) {
        cell.textLabel.text = @"Restore Defaults";
        return cell;
    }
    else if (indexPath.section == 0) {
        name = @[
            @"Greyscale",
            @"Lime (Game Boy)",
            @"Olive (Pocket)",
            @"Teal (Light)",
        ][indexPath.row];
    }
    else {
        name = _cacheNames[indexPath.row];
    }
    
    cell.textLabel.text = name;
    if ([[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"] isEqual:name]) {
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
    }

    cell.imageView.image = [self.class previewImageForTheme:name];
    return cell;

}

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 2) {
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Restore default palettes?"
                                                                       message: @"The defaults palettes will be restored, changes will be reverted, and created or imported palettes will be deleted. This change cannot be undone."
                                                                preferredStyle:UIAlertControllerStyleAlert];
        [alert addAction:[UIAlertAction actionWithTitle:@"Restore"
                                                  style:UIAlertActionStyleDestructive
                                                handler:^(UIAlertAction *action) {
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"GBCurrentTheme"];
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"GBThemes"];
            [self.tableView reloadData];
        }]];
        [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                                  style:UIAlertActionStyleCancel
                                                handler:nil]];
        [self presentViewController:alert animated:true completion:nil];

    }
    [[NSUserDefaults standardUserDefaults] setObject:[self.tableView cellForRowAtIndexPath:indexPath].textLabel.text
                                              forKey:@"GBCurrentTheme"];
    [self.tableView reloadData];
    return nil;
}


- (NSString *)title
{
    return @"Monochrome Palette";
}

- (void)renameRow:(NSIndexPath *)indexPath
{
    if (indexPath.section != 1) return;
    
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
    
    NSArray *builtins = @[
        @"Greyscale",
        @"Lime (Game Boy)",
        @"Olive (Pocket)",
        @"Teal (Light)",
    ];
    
    NSMutableDictionary *dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBThemes"].mutableCopy;
    if (dict[newName] || [builtins containsObject:newName]) {
        unsigned i = 2;
        while (true) {
            NSString *attempt = [NSString stringWithFormat:@"%@ %u", newName, i];
            if (!dict[attempt] && ![builtins containsObject:newName]) {
                newName = attempt;
                break;
            }
            i++;
        }
    }
    
    dict[newName] = dict[oldName];
    [dict removeObjectForKey:oldName];
    [[NSUserDefaults standardUserDefaults] setObject:dict forKey:@"GBThemes"];
    if ([[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"] isEqual:oldName]) {
        [[NSUserDefaults standardUserDefaults] setObject:newName forKey:@"GBCurrentTheme"];
    }
    [self.tableView reloadData];
    _renamingPath = nil;
}


- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section != 1) return;
    
    if (editingStyle != UITableViewCellEditingStyleDelete) return;
    NSString *rom = [self.tableView cellForRowAtIndexPath:indexPath].textLabel.text;
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"Delete “%@”?", rom]
                                                                   message: @"This change cannot be undone."
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"Delete"
                                              style:UIAlertActionStyleDestructive
                                            handler:^(UIAlertAction *action) {
        NSMutableDictionary *dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBThemes"].mutableCopy;
        NSString *name = _cacheNames[indexPath.row];
        [dict removeObjectForKey:name];
        [[NSUserDefaults standardUserDefaults] setObject:dict forKey:@"GBThemes"];
        [self.tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];

        if ([[[NSUserDefaults standardUserDefaults] stringForKey:@"GBCurrentTheme"] isEqual:name]) {
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"GBCurrentTheme"];
            [self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:1 inSection:0]] withRowAnimation:UITableViewRowAnimationFade];
        }
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"Cancel"
                                              style:UIAlertActionStyleCancel
                                            handler:nil]];
    [self presentViewController:alert animated:true completion:nil];
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return indexPath.section == 1;
}

// Leave these ROM management to iOS 13.0 and up for now
- (UIContextMenuConfiguration *)tableView:(UITableView *)tableView
contextMenuConfigurationForRowAtIndexPath:(NSIndexPath *)indexPath
                                    point:(CGPoint)point API_AVAILABLE(ios(13.0))
{
    if (indexPath.section != 1) return nil;
    
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
        return [UIMenu menuWithTitle:nil children:@[
            [UIAction actionWithTitle:@"Rename"
                                image:[UIImage systemImageNamed:@"pencil"]
                           identifier:nil
                              handler:^(__kindof UIAction *action) {
                [self renameRow:indexPath];
            }],
            deleteAction,
        ]];
    }];
}

@end
