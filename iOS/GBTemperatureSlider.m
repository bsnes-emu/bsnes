#import "GBTemperatureSlider.h"

static inline void temperature_tint(double temperature, double *r, double *g, double *b)
{
    if (temperature >= 0) {
        *r = 1;
        *g = pow(1 - temperature, 0.375);
        if (temperature >= 0.75) {
            *b = 0;
        }
        else {
            *b = sqrt(0.75 - temperature) / sqrt(0.75);
        }
    }
    else {
        *b = 1;
        double squared = pow(temperature, 2);
        *g = 0.125 * squared + 0.3 * temperature + 1.0;
        *r = 0.21875 * squared + 0.5 * temperature + 1.0;
    }
}

@implementation GBTemperatureSlider

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self addTarget:self action:@selector(valueChanged) forControlEvents:UIControlEventValueChanged];
    return self;
}

- (void)updateTint
{
    double r, g, b;
    temperature_tint(self.value, &r, &g, &b);
    self.thumbTintColor = [UIColor colorWithRed:r green:g blue:b alpha:1.0];
}

- (void)setValue:(float)value
{
    [super setValue:value];
    [self updateTint];
}

- (void)valueChanged
{
    if (fabsf(self.value) < 0.05 && self.value != 0) {
        self.value = 0;
    }
    else {
        [self updateTint];
    }
}

-(UIImage *)maximumTrackImageForState:(UIControlState)state
{
    return [[UIImage alloc] init];
}


-(UIImage *)minimumTrackImageForState:(UIControlState)state
{
    return [[UIImage alloc] init];
}

- (void)drawRect:(CGRect)rect
{
    CGSize size = self.bounds.size;
    UIBezierPath *path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(2, round(size.height / 2 - 1.5), size.width - 4, 3) cornerRadius:4];
    [path appendPath:[UIBezierPath bezierPathWithRoundedRect:CGRectMake(round(size.width / 2 - 1.5), 12, 3, size.height - 24) cornerRadius:4]];
    [[UIColor colorWithRed:120 / 255.0
                     green:120 / 255.0
                      blue:130 / 255.0
                     alpha:70 / 255.0] set];
    [path fill];
    
    [super drawRect:rect];
}

@end
