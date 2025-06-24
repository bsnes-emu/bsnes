#import "GBToolbarFieldCell.h"
#import <objc/runtime.h>

@interface NSTextFieldCell()
- (void)textDidChange:(id)sender;
@end

@implementation GBToolbarFieldCell

- (void)textDidChange:(id)sender
{
    IMP imp = [NSTextFieldCell instanceMethodForSelector:_cmd];
    method_setImplementation(class_getInstanceMethod([GBToolbarFieldCell class], _cmd), imp);
    ((void(*)(id, SEL, id))imp)(self, _cmd, sender);
}

- (void)endEditing:(NSText *)textObj
{
    IMP imp = [NSTextFieldCell instanceMethodForSelector:_cmd];
    method_setImplementation(class_getInstanceMethod([GBToolbarFieldCell class], _cmd), imp);
    ((void(*)(id, SEL, id))imp)(self, _cmd, textObj);
}
@end
