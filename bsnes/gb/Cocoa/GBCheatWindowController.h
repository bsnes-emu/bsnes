#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "Document.h"

@interface GBCheatWindowController : NSObject <NSTableViewDelegate, NSTableViewDataSource, NSTextFieldDelegate>
@property (nonatomic, weak) IBOutlet NSTableView *cheatsTable;
@property (nonatomic, weak) IBOutlet NSTextField *addressField;
@property (nonatomic, weak) IBOutlet NSTextField *valueField;
@property (nonatomic, weak) IBOutlet NSTextField *oldValueField;
@property (nonatomic, weak) IBOutlet NSButton *oldValueCheckbox;
@property (nonatomic, weak) IBOutlet NSTextField *descriptionField;
@property (nonatomic, weak) IBOutlet NSTextField *importCodeField;
@property (nonatomic, weak) IBOutlet NSTextField *importDescriptionField;
@property (nonatomic, weak) IBOutlet Document *document;
- (void)cheatsUpdated;
@end
