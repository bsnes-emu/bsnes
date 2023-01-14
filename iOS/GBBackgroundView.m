#import "GBBackgroundView.h"
#import "GBViewMetal.h"
#import "GBHapticManager.h"

double CGPointSquaredDistance(CGPoint a, CGPoint b)
{
    double deltaX = a.x - b.x;
    double deltaY = a.y - b.y;
    return deltaX * deltaX + deltaY * deltaY;
}

double CGPointAngle(CGPoint a, CGPoint b)
{
    double deltaX = a.x - b.x;
    double deltaY = a.y - b.y;
    return atan2(deltaY, deltaX);
}

static void positionView(UIImageView *view, CGPoint position)
{
    double center = view.image.size.width / 2 * [UIScreen mainScreen].scale;
    view.frame = (CGRect){
        {
            round(position.x - center) / [UIScreen mainScreen].scale,
            round(position.y - center) / [UIScreen mainScreen].scale
        },
        view.image.size
    };
}

@implementation GBBackgroundView
{
    NSMutableSet<UITouch *> *_touches;
    UIImageView *_dpadView;
    UIImageView *_aButtonView;
    UIImageView *_bButtonView;
    UIImageView *_startButtonView;
    UIImageView *_selectButtonView;
    GB_key_mask_t _lastMask;
}

- (instancetype)init
{
    self = [super initWithImage:nil];
    if (!self) return nil;
    _touches = [NSMutableSet set];
    _dpadView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"dpad"]];
    _aButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _bButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button"]];
    _startButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    _selectButtonView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"button2"]];
    _gbView = [[GBViewMetal alloc] initWithFrame:CGRectZero];
    
    [self addSubview:_dpadView];
    [self addSubview:_aButtonView];
    [self addSubview:_bButtonView];
    [self addSubview:_startButtonView];
    [self addSubview:_selectButtonView];
    [self addSubview:_gbView];
    return self;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [_touches unionSet:touches];
    [self touchesChanged];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [_touches minusSet:touches];
    [self touchesChanged];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [_touches minusSet:touches];
    [self touchesChanged];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self touchesChanged];
}

- (void)touchesChanged
{
    if (!GB_is_inited(_gbView.gb)) return;
    GB_key_mask_t mask = 0;
    double factor = [UIScreen mainScreen].scale;
    double buttonRadiusSquared = 36 *  36 * factor * factor;
    double dpadRadiusSquared = 75 *  75 * factor * factor;
    for (UITouch *touch in _touches) {
        CGPoint point = [touch locationInView:self];
        point.x *= factor;
        point.y *= factor;
        if (CGPointSquaredDistance(point, _layout.aLocation) <= buttonRadiusSquared) {
            mask |= GB_KEY_A_MASK;
        }
        else if (CGPointSquaredDistance(point, _layout.bLocation) <= buttonRadiusSquared) {
            mask |= GB_KEY_B_MASK;
        }
        else if (CGPointSquaredDistance(point, _layout.startLocation) <= buttonRadiusSquared) {
            mask |= GB_KEY_START_MASK;
        }
        else if (CGPointSquaredDistance(point, _layout.selectLocation) <= buttonRadiusSquared) {
            mask |= GB_KEY_SELECT_MASK;
        }
        else if (CGPointSquaredDistance(point, _layout.dpadLocation) <= dpadRadiusSquared) {
            double angle = CGPointAngle(point, _layout.dpadLocation);
            signed quantizedAngle = round(angle / M_PI * 6);
            if (quantizedAngle < 0) {
                quantizedAngle += 12;
            }
            switch (quantizedAngle) {
                case 0 : mask |= GB_KEY_RIGHT_MASK; break;
                case 1 : mask |= GB_KEY_RIGHT_MASK | GB_KEY_DOWN_MASK; break;
                case 2 : mask |= GB_KEY_DOWN_MASK; break;
                    
                case 3 : mask |= GB_KEY_DOWN_MASK; break;
                case 4 : mask |= GB_KEY_LEFT_MASK | GB_KEY_DOWN_MASK; break;
                case 6 : mask |= GB_KEY_LEFT_MASK; break;
                    
                case 7 : mask |= GB_KEY_LEFT_MASK; break;
                case 8 : mask |= GB_KEY_LEFT_MASK | GB_KEY_UP_MASK; break;
                case 9 : mask |= GB_KEY_UP_MASK; break;
                    
                case 10: mask |= GB_KEY_UP_MASK; break;
                case 11: mask |= GB_KEY_RIGHT_MASK | GB_KEY_UP_MASK; break;
                case 12: mask |= GB_KEY_RIGHT_MASK; break;
            }
        }
    }
    GB_set_key_mask(_gbView.gb, mask);
    if (mask & ~_lastMask) {
        [[GBHapticManager sharedManager] doTapHaptic];
    }
    _lastMask = mask;
}

- (BOOL)isMultipleTouchEnabled
{
    return true;
}

- (BOOL)isUserInteractionEnabled
{
    return true;
}

- (void)setLayout:(GBLayout *)layout
{
    _layout = layout;
    self.image = layout.background;
    
    positionView(_dpadView, layout.dpadLocation);
    positionView(_aButtonView, layout.aLocation);
    positionView(_bButtonView, layout.bLocation);
    positionView(_startButtonView, layout.startLocation);
    positionView(_selectButtonView, layout.selectLocation);
    
    CGRect screenFrame = layout.screenRect;
    screenFrame.origin.x /= [UIScreen mainScreen].scale;
    screenFrame.origin.y /= [UIScreen mainScreen].scale;
    screenFrame.size.width /= [UIScreen mainScreen].scale;
    screenFrame.size.height /= [UIScreen mainScreen].scale;
    
    _gbView.frame = screenFrame;
}

@end
