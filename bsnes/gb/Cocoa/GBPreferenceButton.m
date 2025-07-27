#import "GBPreferenceButton.h"
#import "NSObject+DefaultsObserver.h"

@implementation GBPreferenceButton

- (BOOL)sendAction:(SEL)action to:(id)target
{
    [[NSUserDefaults standardUserDefaults] setBool:self.state ^ self.invertValue forKey:_preferenceName];
    return [super sendAction:action to:target];
}

- (void)updateValue
{
    if (!_preferenceName) return;
    self.state = [[NSUserDefaults standardUserDefaults] boolForKey:_preferenceName]  ^ self.invertValue;
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
