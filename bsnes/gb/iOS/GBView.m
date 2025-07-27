#import "GBView.h"

@implementation GBView
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self createInternalView];
    [self addSubview:self.internalView];
    self.internalView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    return self;
}
@end
