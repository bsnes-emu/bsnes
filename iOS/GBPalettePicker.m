#import "GBPalettePicker.h"

@interface GBPalettePicker ()
{
    NSArray <NSString *>* _cacheNames;
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

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0) return 4;
    return [[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBThemes"].count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    
    NSString *name = nil;
    if (indexPath.section == 0) {
        name = @[
            @"Greyscale",
            @"Lime (Game Boy)",
            @"Olive (Pocket)",
            @"Teal (Light)",
        ][indexPath.row];
    }
    else {
        if (!_cacheNames) {
            _cacheNames = [[[NSUserDefaults standardUserDefaults] dictionaryForKey:@"GBThemes"].allKeys sortedArrayUsingSelector:@selector(localizedCaseInsensitiveCompare:)];
        }
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
    
    [[NSUserDefaults standardUserDefaults] setObject:[self.tableView cellForRowAtIndexPath:indexPath].textLabel.text
                                              forKey:@"GBCurrentTheme"];
    [self.tableView reloadData];
    return nil;
}


- (NSString *)title
{
    return @"Monochrome Palette";
}

@end
