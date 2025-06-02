#import "GBThemesViewController.h"
#import "GBThemePreviewController.h"
#import "GBTheme.h"

@interface GBThemesViewController ()

@end

@implementation GBThemesViewController
{
    NSArray<NSArray<GBTheme *> *> *_themes;
}

+ (NSArray<NSArray<GBTheme *> *> *)themes
{
    static __weak NSArray<NSArray<GBTheme *> *> *cache = nil;
    if (cache) return cache;
    id ret  = @[
        @[
            [[GBTheme alloc] initDefaultTheme],
            [[GBTheme alloc] initDarkTheme],
        ],
    ];
    cache = ret;
    return ret;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _themes = [[self class] themes];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return _themes.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _themes[section].count;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    GBTheme *theme = _themes[indexPath.section][indexPath.row];
    cell.textLabel.text = theme.name;

    cell.accessoryType = [[[NSUserDefaults standardUserDefaults] stringForKey:@"GBInterfaceTheme"] isEqual:theme.name]? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;
    bool horizontal = self.interfaceOrientation >= UIInterfaceOrientationLandscapeRight;
    UIImage *preview = horizontal? [theme horizontalPreview] : [theme verticalPreview];
    UIGraphicsBeginImageContextWithOptions((CGSize){60, 60}, false, self.view.window.screen.scale);
    unsigned width = 60;
    unsigned height = 56;
    if (horizontal) {
        height = round(preview.size.height / preview.size.width * 60);
    }
    else {
        width = round(preview.size.width / preview.size.height * 56);
    }
    UIBezierPath *mask = [UIBezierPath bezierPathWithRoundedRect:CGRectMake((60 - width) / 2, (60 - height) / 2, width, height) cornerRadius:4];
    [mask addClip];
    [preview drawInRect:mask.bounds];
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

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [self.tableView reloadData];
    [super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    GBTheme *theme = _themes[indexPath.section][indexPath.row];
    GBThemePreviewController *preview = [[GBThemePreviewController alloc] initWithTheme:theme];
    [self presentViewController:preview animated:true completion:nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self.tableView reloadData];
    [super viewWillAppear:animated];
}

@end
