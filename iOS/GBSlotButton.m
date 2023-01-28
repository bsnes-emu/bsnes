#import "GBSlotButton.h"

@implementation GBSlotButton
{
    UIImageView *_imageView;
}

+ (instancetype)buttonWithLabelText:(NSString *)labelText
{
    GBSlotButton *ret = [self buttonWithType:UIButtonTypeCustom];
    if (!ret) return nil;
    ret.frame = CGRectMake(0, 0, 0x100, 0x100);
    
    ret->_slotSubtitleLabel = [[UILabel alloc] init];
    ret->_slotSubtitleLabel.text = @"Empty";
    ret->_slotSubtitleLabel.font = [UIFont systemFontOfSize:UIFont.smallSystemFontSize];
    if (@available(iOS 13.0, *)) {
        ret->_slotSubtitleLabel.textColor = [UIColor secondaryLabelColor];
    }
    else {
        ret->_slotSubtitleLabel.textColor = [UIColor systemGrayColor];
    }
    [ret->_slotSubtitleLabel sizeToFit];
    ret->_slotSubtitleLabel.textAlignment = NSTextAlignmentCenter;
    CGRect slotSubtitleLabelRect = ret->_slotSubtitleLabel.frame;
    slotSubtitleLabelRect.size.width = 0x100;
    slotSubtitleLabelRect.origin.y = 0x100 - slotSubtitleLabelRect.size.height - 8;
    ret->_slotSubtitleLabel.frame = slotSubtitleLabelRect;
    ret->_slotSubtitleLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleTopMargin;
    [ret addSubview:ret->_slotSubtitleLabel];
    
    ret->_label = [[UILabel alloc] init];
    ret->_label.text = labelText;
    [ret->_label sizeToFit];
    ret->_label.textAlignment = NSTextAlignmentCenter;
    CGRect labelRect = ret->_label.frame;
    labelRect.size.width = 0x100;
    labelRect.origin.y = slotSubtitleLabelRect.origin.y - labelRect.size.height - 4;
    ret->_label.frame = labelRect;
    ret->_label.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleTopMargin;
    [ret addSubview:ret->_label];
    
    ret.backgroundColor = [UIColor clearColor];
    
    ret->_imageView = [[UIImageView alloc] initWithImage:nil];
    ret.imageView.layer.cornerRadius = 6;
    ret.imageView.layer.masksToBounds = true;
    if (@available(iOS 13.0, *)) {
        ret.imageView.layer.borderColor = [UIColor tertiaryLabelColor] .CGColor;
    }
    else {
        ret.imageView.layer.borderColor = [UIColor colorWithWhite:0 alpha:0.5].CGColor;
    }
    ret.imageView.layer.borderWidth = 1;
    ret.imageView.layer.backgroundColor = [UIColor whiteColor].CGColor;
    [ret addSubview:ret.imageView];
    
    return ret;
}

- (UIImageView *)imageView
{
    return _imageView;
}

- (void)setHighlighted:(BOOL)highlighted
{
    if (_showingMenu) return;
    if (highlighted == self.isHighlighted) return;
    [super setHighlighted:highlighted];
    
    [UIView animateWithDuration:0.25 animations:^{
        if (highlighted) {
            if (@available(iOS 13.0, *)) {
                self.backgroundColor = [UIColor tertiarySystemFillColor];
            }
            else {
                self.backgroundColor = [UIColor colorWithRed:118 / 255.0
                                                       green:118 / 255.0
                                                        blue:128 / 255.0
                                                       alpha:0.12];
            }
            self.imageView.layer.opacity = 11 / 12.0;
        }
        else {
            self.backgroundColor = [UIColor clearColor];
            self.imageView.layer.opacity = 1;
        }
    }];
}

- (void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    CGRect screenshotRect = self.bounds;
    screenshotRect.size.width -= 8;
    screenshotRect.origin.x += 4;
    screenshotRect.size.height = _label.frame.origin.y - 16;
    screenshotRect.origin.y += 8;
    
    double scale = [UIApplication sharedApplication].keyWindow.screen.scale;
    double nativeWidth = 160.0 / scale;
    double nativeHeight = 144.0 / scale;
    
    if (screenshotRect.size.width > nativeWidth &&
        screenshotRect.size.height > nativeHeight) {
        self.imageView.layer.magnificationFilter = kCAFilterNearest;
        double newWidth = floor(screenshotRect.size.width / nativeWidth) * nativeWidth;
        screenshotRect.origin.x += (screenshotRect.size.width - newWidth) / 2;
        screenshotRect.size.width = newWidth;
        
        double newHeight = floor(screenshotRect.size.height / nativeHeight) * nativeHeight;
        screenshotRect.origin.y += (screenshotRect.size.height - newHeight) / 2;
        screenshotRect.size.height = newHeight;
    }
    else {
        self.imageView.layer.magnificationFilter = kCAFilterLinear;
    }
    
    double aspect = screenshotRect.size.width / screenshotRect.size.height;
    if (aspect > 160.0 / 144.0) {
        // Too wide
        double newWidth = screenshotRect.size.height / 144 * 160;
        screenshotRect.origin.x += (screenshotRect.size.width - newWidth) / 2;
        screenshotRect.size.width = newWidth;
    }
    else {
        // Too narrow
        double newHeight = screenshotRect.size.width / 160 * 144;
        screenshotRect.origin.y += (screenshotRect.size.height - newHeight) / 2;
        screenshotRect.size.height = newHeight;
    }
    screenshotRect.origin.x = round(screenshotRect.origin.x);
    screenshotRect.origin.y = round(screenshotRect.origin.y);
    self.imageView.frame = screenshotRect;
}

- (void)setShowingMenu:(bool)showingMenu
{
    if (showingMenu) {
        self.highlighted = true;
        _showingMenu = true;
    }
    else {
        _showingMenu = false;
        self.highlighted = false;
    }
}

@end
