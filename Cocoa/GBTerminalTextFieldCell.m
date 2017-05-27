#import <Carbon/Carbon.h>
#import "GBTerminalTextFieldCell.h"

@interface GBTerminalTextView : NSTextView
@end

@implementation GBTerminalTextFieldCell
{
    GBTerminalTextView *field_editor;
}

- (NSTextView *)fieldEditorForView:(NSView *)controlView
{
    if (field_editor) {
        return field_editor;
    }
    field_editor = [[GBTerminalTextView alloc] init];
    [field_editor setFieldEditor:YES];
    return field_editor;
}

@end

@implementation GBTerminalTextView
{
    NSMutableOrderedSet *lines;
    NSUInteger current_line;
}

- (instancetype)init
{
    self = [super init];
    if (!self) {
        return NULL;
    }
    lines = [[NSMutableOrderedSet alloc] init];
    return self;
}

- (void)moveUp:(id)sender
{
    if (current_line != 0) {
        current_line--;
        [self setString:[lines objectAtIndex:current_line]];
    }
}

- (void)moveDown:(id)sender
{
    if (current_line == [lines count]) {
        return;
    }
    current_line++;
    if (current_line == [lines count]) {
        [self setString:@""];
    }
    else {
        [self setString:[lines objectAtIndex:current_line]];
    }
}

-(void) insertNewline:(id)sender
{
    if ([self.string length]) {
        NSString *string = [self.string copy];
        [lines removeObject:string];
        [lines addObject:string];
    }
    [super insertNewline:sender];
    current_line = [lines count];
}

@end
