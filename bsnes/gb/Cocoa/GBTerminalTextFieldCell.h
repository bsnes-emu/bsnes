#import <Cocoa/Cocoa.h>
#import <Core/gb.h>

@interface GBTerminalTextFieldCell : NSTextFieldCell
@property (nonatomic) GB_gameboy_t *gb;
@end
