#import "NSString+StringForKey.h"
#import "KeyboardShortcutPrivateAPIs.h"
#import <Carbon/Carbon.h>

@implementation NSString (StringForKey)

+ (NSString *) displayStringForKeyString: (NSString *)key_string
{
    return [[NSKeyboardShortcut shortcutWithKeyEquivalent:key_string modifierMask:0] localizedDisplayName];
}

+ (NSString *) displayStringForKeyCode:(unsigned short) keyCode
{
    /* These cases are not handled by stringForVirtualKey */
    switch (keyCode) {
        case 115: return @"↖";
        case 119: return @"↘";
        case 116: return @"⇞";
        case 121: return @"⇟";
        case 51: return @"⌫";
        case 117: return @"⌦";
        case 76: return @"⌤";

        /* Label Keypad buttons accordingly */
        default:
            if ((keyCode < 82 || keyCode > 92)) {
                return [NSPrefPaneUtils stringForVirtualKey:keyCode modifiers:0];
            }
            
        case 65: case 67: case 69: case 75: case 78: case 81:
            return [@"Keypad " stringByAppendingString:[NSPrefPaneUtils stringForVirtualKey:keyCode modifiers:0]];
    }
}

@end
