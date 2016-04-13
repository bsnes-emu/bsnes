#ifndef GBButtons_h
#define GBButtons_h

typedef enum : NSUInteger {
    GBRight,
    GBLeft,
    GBUp,
    GBDown,
    GBA,
    GBB,
    GBSelect,
    GBStart,
    GBTurbo,
    GBButtonCount
} GBButton;

extern NSString const *GBButtonNames[GBButtonCount];

static inline NSString *button_to_preference_name(GBButton button)
{
    return [NSString stringWithFormat:@"GB%@", GBButtonNames[button]];
}

#endif
