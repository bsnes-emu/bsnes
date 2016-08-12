#import <Cocoa/Cocoa.h>
#include "GBView.h"

@interface Document : NSDocument <NSWindowDelegate>
@property (strong) IBOutlet GBView *view;
@property (strong) IBOutlet NSTextView *consoleOutput;
@property (strong) IBOutlet NSPanel *consoleWindow;
@property (strong) IBOutlet NSTextField *consoleInput;
@property (strong) IBOutlet NSWindow *mainWindow;
@property (strong) IBOutlet NSView *memoryView;
@property (strong) IBOutlet NSPanel *memoryWindow;

-(uint8_t) readMemory:(uint16_t) addr;
-(void) writeMemory:(uint16_t) addr value:(uint8_t)value;
-(void) performAtomicBlock: (void (^)())block;

@end

