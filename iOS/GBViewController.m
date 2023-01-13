#import "GBViewController.h"
#import "GBHorizontalLayout.h"
#import "GBVerticalLayout.h"

static void positionView(UIImageView *view, CGPoint position)
{
    double center = view.image.size.width / 2 * [UIScreen mainScreen].scale;
    view.frame = (CGRect){
        {
            round(position.x - center) / [UIScreen mainScreen].scale,
            round(position.y - center) / [UIScreen mainScreen].scale
        },
        view.image.size
    };
    
}

@implementation GBViewController
{
    GBLayout *_currentLayout;
    GBHorizontalLayout *_horizontalLayout;
    GBVerticalLayout *_verticalLayout;
    UIImageView *_backgroundView;
    UIImageView *_dpadView;
    UIImageView *_aButtonView;
    UIImageView *_bButtonView;
    UIImageView *_startButtonView;
    UIImageView *_selectButtonView;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _window = [[UIWindow alloc] init];
    _window.rootViewController = self;
    [_window makeKeyAndVisible];
    _horizontalLayout = [[GBHorizontalLayout alloc] init];
    _verticalLayout = [[GBVerticalLayout alloc] init];
    
    _backgroundView = [[UIImageView alloc] initWithImage:nil];
    [_window addSubview:_backgroundView];
    
    _dpadView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"dpad"]];
    _aButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _bButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _startButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    _selectButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    
    [_backgroundView addSubview:_dpadView];
    [_backgroundView addSubview:_aButtonView];
    [_backgroundView addSubview:_bButtonView];
    [_backgroundView addSubview:_startButtonView];
    [_backgroundView addSubview:_selectButtonView];
    
    [self orientationChange];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientationChange)
                                                 name:UIApplicationDidChangeStatusBarOrientationNotification
                                               object:nil];
    return true;
}

- (void)orientationChange
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) {
        _currentLayout = _verticalLayout;
    }
    else {
        _currentLayout = _horizontalLayout;
    }
    
    _backgroundView.image = _currentLayout.background;
    _backgroundView.frame = [_currentLayout viewRectForOrientation:[UIApplication sharedApplication].statusBarOrientation];
    
    positionView(_dpadView, _currentLayout.dpadLocation);
    positionView(_aButtonView, _currentLayout.aLocation);
    positionView(_bButtonView, _currentLayout.bLocation);
    positionView(_startButtonView, _currentLayout.startLocation);
    positionView(_selectButtonView, _currentLayout.selectLocation);
}

- (BOOL)prefersHomeIndicatorAutoHidden
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) {
        return false;
    }
    return true;
}

@end
