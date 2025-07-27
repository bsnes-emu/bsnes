#import "GBMenuButton.h"

@implementation GBMenuButton

- (void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    if (!self.imageView.image) return;
    CGSize imageSize = self.imageView.frame.size;
    CGSize titleSize = self.titleLabel.frame.size;
    
    self.imageEdgeInsets = UIEdgeInsetsMake(0,
                                            0,
                                            28,
                                            -titleSize.width);
    
    self.titleEdgeInsets = UIEdgeInsetsMake(36,
                                            -imageSize.width,
                                            0,
                                            0);
}

@end
