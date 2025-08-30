#import "GBPreferencePopUpButton.h"
#import "NSObject+DefaultsObserver.h"

@implementation GBPreferenceMenuItem
@end

@implementation GBPreferencePopUpButton

- (BOOL)sendAction:(SEL)action to:(id)target
{
    GBPreferenceMenuItem *item = (GBPreferenceMenuItem *)self.selectedItem;
    if ([item isKindOfClass:[GBPreferenceMenuItem class]]) {
        [[NSUserDefaults standardUserDefaults] setObject:item.preferenceValue forKey:_preferenceName];
    }
    else {
        [[NSUserDefaults standardUserDefaults] setInteger:item.tag forKey:_preferenceName];
    }
    return [super sendAction:action to:target];
}

- (void)updateValue
{
    if (!_preferenceName) return;
    NSString *stringValue = [[NSUserDefaults standardUserDefaults]  objectForKey:_preferenceName];
    if ([stringValue isKindOfClass:[NSString class]]) {
        for (GBPreferenceMenuItem *item in self.menu.itemArray) {
            if ([item isKindOfClass:[GBPreferenceMenuItem class]] &&
                [item.preferenceValue isEqualToString:stringValue]) {
                [self selectItem:item];
                return;
            }
        }
    }
    else {
        [self selectItemWithTag:[[NSUserDefaults standardUserDefaults] integerForKey:_preferenceName]];
    }
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
