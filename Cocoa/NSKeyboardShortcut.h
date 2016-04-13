#ifndef NSKeyboardShortcut_h
#define NSKeyboardShortcut_h

/* This is private API, but it is a very simple and comprehensive way
   to convert a key equivalent to its display name. */

@interface NSKeyboardShortcut : NSObject <NSCopying>

+ (id)shortcutWithPreferencesEncoding:(NSString *)encoding;
+ (id)shortcutWithKeyEquivalent:(NSString *)key_equivalent modifierMask:(unsigned long long)mask;
- (id)initWithKeyEquivalent:(NSString *)key_equivalent modifierMask:(unsigned long long)mask;

@property(readonly) unsigned long long modifierMask;
@property(readonly) NSString *keyEquivalent;
@property(readonly) NSString *preferencesEncoding;
@property(readonly) NSString *localizedModifierMaskDisplayName;
@property(readonly) NSString *localizedKeyEquivalentDisplayName;
@property(readonly) NSString *localizedDisplayName;

@end

#endif