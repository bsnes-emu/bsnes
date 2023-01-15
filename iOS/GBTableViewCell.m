#import "GBTableViewCell.h"

@implementation GBTableViewCell

-(void )layoutSubviews
{
    [super layoutSubviews];
    self.imageView.frame = CGRectInset(self.imageView.frame, 0, self.padding);
}

@end
