#import "NSString+StringForKey.h"
#import "NSKeyboardShortcut.h"

@implementation NSString (StringForKey)

+ (NSString *) displayStringForKeyString: (NSString *)key_string
{

    return [[NSKeyboardShortcut shortcutWithKeyEquivalent:key_string modifierMask:0] localizedDisplayName];
}

@end
