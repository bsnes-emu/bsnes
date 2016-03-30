#import <Cocoa/Cocoa.h>
#include "GBView.h"

@interface Document : NSDocument <NSWindowDelegate>
@property (strong) IBOutlet GBView *view;
@property (strong) IBOutlet NSTextView *consoleOutput;
@property (strong) IBOutlet NSPanel *consoleWindow;
@property (strong) IBOutlet NSTextField *consoleInput;


@end

