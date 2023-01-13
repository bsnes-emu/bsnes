#import "GBViewController.h"
#import "GBHorizontalLayout.h"
#import "GBVerticalLayout.h"
#import "GBViewMetal.h"
#include <Core/gb.h>

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
    GBView *_gbView;
    GB_gameboy_t _gb;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _window = [[UIWindow alloc] init];
    _window.rootViewController = self;
    [_window makeKeyAndVisible];
    
    _window.backgroundColor = [UIColor colorWithRed:174 / 255.0 green:176 / 255.0 blue:180 / 255.0 alpha:1.0];
    
    _horizontalLayout = [[GBHorizontalLayout alloc] init];
    _verticalLayout = [[GBVerticalLayout alloc] init];
    
    _backgroundView = [[UIImageView alloc] initWithImage:nil];
    [_window addSubview:_backgroundView];
    self.view = _backgroundView;
    
    _dpadView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"dpad"]];
    _aButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _bButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _startButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    _selectButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    _gbView = [[GBViewMetal alloc] initWithFrame:CGRectZero];
    
    GB_init(&_gb, GB_MODEL_CGB_E);
    _gbView.gb = &_gb;
    [_gbView screenSizeChanged];
    
    [self willRotateToInterfaceOrientation:[UIApplication sharedApplication].statusBarOrientation
                                  duration:0];
    
    [_backgroundView addSubview:_dpadView];
    [_backgroundView addSubview:_aButtonView];
    [_backgroundView addSubview:_bButtonView];
    [_backgroundView addSubview:_startButtonView];
    [_backgroundView addSubview:_selectButtonView];
    [_backgroundView addSubview:_gbView];
    
    return true;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)orientation duration:(NSTimeInterval)duration
{
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
    
    CGRect screenFrame = _currentLayout.screenRect;
    screenFrame.origin.x /= [UIScreen mainScreen].scale;
    screenFrame.origin.y /= [UIScreen mainScreen].scale;
    screenFrame.size.width /= [UIScreen mainScreen].scale;
    screenFrame.size.height /= [UIScreen mainScreen].scale;
    
    _gbView.frame = screenFrame;
    memset(_gbView.pixels, rand(), 160 * 144 * 4);
    [_gbView flip];
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
