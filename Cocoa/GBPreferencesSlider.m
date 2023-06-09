#import "GBPreferencesSlider.h"
#import "NSObject+DefaultsObserver.h"

@implementation GBPreferencesSlider

- (BOOL)sendAction:(SEL)action to:(id)target
{
    [[NSUserDefaults standardUserDefaults] setDouble:self.doubleValue / (self.denominator ?: 1) forKey:_preferenceName];
    return [super sendAction:action to:target];
}

- (void)updateValue
{
    if (!_preferenceName) return;
    self.doubleValue = [[NSUserDefaults standardUserDefaults] doubleForKey:_preferenceName] * (self.denominator ?: 1);
}

- (void)setPreferenceName:(NSString *)preferenceName
{
    _preferenceName = preferenceName;
    [self observeStandardDefaultsKey:_preferenceName selector:@selector(updateValue)];
}

- (void)viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
    [self updateValue];
}
@end
