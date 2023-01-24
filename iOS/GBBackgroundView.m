#import "GBBackgroundView.h"
#import "GBViewMetal.h"
#import "GBHapticManager.h"
#import "GBMenuViewController.h"

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

static GB_key_mask_t angleToKeyMask(double angle)
{
    signed quantizedAngle = round(angle / M_PI * 16);
    if (quantizedAngle < 0) {
        quantizedAngle += 32;
    }
    switch (quantizedAngle) {
        case 32:
        case  0: return GB_KEY_RIGHT_MASK;
        case  1: return GB_KEY_RIGHT_MASK;
        case  2: return GB_KEY_RIGHT_MASK;
        case  3: return GB_KEY_RIGHT_MASK | GB_KEY_DOWN_MASK;
        case  4: return GB_KEY_RIGHT_MASK | GB_KEY_DOWN_MASK;
        case  5: return GB_KEY_DOWN_MASK;
        case  6: return GB_KEY_DOWN_MASK;
        case  7: return GB_KEY_DOWN_MASK;
            
        case  8: return GB_KEY_DOWN_MASK;
        case  9: return GB_KEY_DOWN_MASK;
        case 10: return GB_KEY_DOWN_MASK;
        case 11: return GB_KEY_LEFT_MASK | GB_KEY_DOWN_MASK;
        case 12: return GB_KEY_LEFT_MASK | GB_KEY_DOWN_MASK;
        case 13: return GB_KEY_LEFT_MASK;
        case 14: return GB_KEY_LEFT_MASK;
        case 15: return GB_KEY_LEFT_MASK;
            
        case 16: return GB_KEY_LEFT_MASK;
        case 17: return GB_KEY_LEFT_MASK;
        case 18: return GB_KEY_LEFT_MASK;
        case 19: return GB_KEY_LEFT_MASK | GB_KEY_UP_MASK;
        case 20: return GB_KEY_LEFT_MASK | GB_KEY_UP_MASK;
        case 21: return GB_KEY_UP_MASK;
        case 22: return GB_KEY_UP_MASK;
        case 23: return GB_KEY_UP_MASK;
            
        case 24: return GB_KEY_UP_MASK;
        case 25: return GB_KEY_UP_MASK;
        case 26: return GB_KEY_UP_MASK;
        case 27: return GB_KEY_RIGHT_MASK | GB_KEY_UP_MASK;
        case 28: return GB_KEY_RIGHT_MASK | GB_KEY_UP_MASK;
        case 29: return GB_KEY_RIGHT_MASK;
        case 30: return GB_KEY_RIGHT_MASK;
        case 31: return GB_KEY_RIGHT_MASK;
    }
    
    return 0;
}

@implementation GBBackgroundView
{
    NSMutableSet<UITouch *> *_touches;
    UITouch *_swipePadTouch;
    CGPoint _swipeOrigin;
    UIImageView *_dpadView;
    UIImageView *_dpadShadowView;
    UIImageView *_aButtonView;
    UIImageView *_bButtonView;
    UIImageView *_startButtonView;
    UIImageView *_selectButtonView;
    UILabel *_screenLabel;
    GB_key_mask_t _lastMask;
}

- (instancetype)init
{
    self = [super initWithImage:nil];
    if (!self) return nil;
    _touches = [NSMutableSet set];
    
    _screenLabel = [[UILabel alloc] initWithFrame:CGRectZero];
    _screenLabel.text = @"Tap the Game Boy screen to open the menu and load a ROM from the library.";
    _screenLabel.font = [UIFont systemFontOfSize:20 weight:UIFontWeightMedium];
    _screenLabel.textAlignment = NSTextAlignmentCenter;
    _screenLabel.textColor = [UIColor whiteColor];
    _screenLabel.lineBreakMode = NSLineBreakByWordWrapping;
    _screenLabel.numberOfLines = 0;
    [self addSubview:_screenLabel];
    
    _dpadView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"dpad"]];
    _dpadShadowView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"dpadShadow"]];
    _dpadShadowView.hidden = true;
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
    
    [_dpadView addSubview:_dpadShadowView];
    return self;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    static const double dpadRadius = 75;
    CGPoint dpadLocation = _layout.dpadLocation;
    double factor = [UIScreen mainScreen].scale;
    dpadLocation.x /= factor;
    dpadLocation.y /= factor;
    for (UITouch *touch in touches) {
        CGPoint point = [touch locationInView:self];
        if (CGRectContainsPoint(self.gbView.frame, point)) {
            [self.window.rootViewController presentViewController:[GBMenuViewController menu] animated:true completion:nil];
        }
        
        if (_usesSwipePad && !_swipePadTouch) {
            if (fabs(point.x - dpadLocation.x) <= dpadRadius &&
                fabs(point.y - dpadLocation.y) <= dpadRadius) {
                _swipePadTouch = touch;
                _swipeOrigin = point;
            }
        }
    }
    [_touches unionSet:touches];
    [self touchesChanged];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if ([touches containsObject:_swipePadTouch]) {
        _swipePadTouch = nil;
    }
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
    double dpadRadius = 75 * factor;
    bool dpadHandled = false;
    if (_usesSwipePad) {
        dpadHandled = true;
        if (_swipePadTouch) {
            CGPoint point = [_swipePadTouch locationInView:self];
            double squaredDistance = CGPointSquaredDistance(point, _swipeOrigin);
            if (squaredDistance > 16 * 16) {
                double angle = CGPointAngle(point, _swipeOrigin);
                mask |= angleToKeyMask(angle);
                if (squaredDistance > 24 * 24) {
                    double deltaX = point.x - _swipeOrigin.x;
                    double deltaY = point.y - _swipeOrigin.y;
                    double distance = sqrt(squaredDistance);
                    _swipeOrigin.x = point.x - deltaX / distance * 24;
                    _swipeOrigin.y = point.y - deltaY / distance * 24;
                }
            }
        }
    }
    for (UITouch *touch in _touches) {
        if (touch == _swipePadTouch) continue;
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
        else if (!dpadHandled &&
                 fabs(point.x - _layout.dpadLocation.x) <= dpadRadius &&
                 fabs(point.y - _layout.dpadLocation.y) <= dpadRadius) {
            dpadHandled = true; // Don't handle the dpad twice
            double angle = CGPointAngle(point, _layout.dpadLocation);
            mask |= angleToKeyMask(angle);
        }
    }
    if (mask != _lastMask) {
        _aButtonView.image      = [UIImage imageNamed:(mask & GB_KEY_A_MASK)?      @"buttonPressed"  : @"button"];
        _bButtonView.image      = [UIImage imageNamed:(mask & GB_KEY_B_MASK)?      @"buttonPressed"  : @"button"];
        _startButtonView.image  = [UIImage imageNamed:(mask & GB_KEY_START_MASK) ? @"button2Pressed" : @"button2"];
        _selectButtonView.image = [UIImage imageNamed:(mask & GB_KEY_SELECT_MASK)? @"button2Pressed" : @"button2"];
        
        bool hidden = false;
        bool diagonal = false;
        double rotation = 0;
        switch (mask & (GB_KEY_RIGHT_MASK | GB_KEY_DOWN_MASK | GB_KEY_LEFT_MASK | GB_KEY_UP_MASK)) {
            case GB_KEY_RIGHT_MASK: break;
            case GB_KEY_RIGHT_MASK | GB_KEY_DOWN_MASK: diagonal = true; break;
            case GB_KEY_DOWN_MASK: rotation = M_PI_2; break;
            case GB_KEY_LEFT_MASK | GB_KEY_DOWN_MASK: diagonal = true; rotation = M_PI_2; break;
            case GB_KEY_LEFT_MASK: rotation = M_PI; break;
            case GB_KEY_LEFT_MASK | GB_KEY_UP_MASK: diagonal = true; rotation = M_PI; break;
            case GB_KEY_UP_MASK: rotation = -M_PI_2; break;
            case GB_KEY_RIGHT_MASK | GB_KEY_UP_MASK: diagonal = true; rotation = -M_PI_2; break;
            default:
                hidden = true;
        }
        
        _dpadShadowView.hidden = hidden;
        if (!hidden) {
            if (_usesSwipePad) {
                _dpadShadowView.image = [UIImage imageNamed:diagonal? @"swipepadShadowDiagonal" : @"swipepadShadow"];

            }
            else {
                _dpadShadowView.image = [UIImage imageNamed:diagonal? @"dpadShadowDiagonal" : @"dpadShadow"];
            }
            _dpadShadowView.transform = CGAffineTransformMakeRotation(rotation);
        }
        
        GB_set_key_mask(_gbView.gb, mask);
        if ((mask & ~_lastMask) && ([[NSUserDefaults standardUserDefaults] boolForKey:@"GBButtonHaptics"])) {
            [[GBHapticManager sharedManager] doTapHaptic];
        }
        _lastMask = mask;
    }
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
    
    screenFrame.origin.x += 8;
    screenFrame.origin.y += 8;
    screenFrame.size.width -= 16;
    screenFrame.size.height -= 16;
    _screenLabel.frame = screenFrame;
}

- (void)setUsesSwipePad:(bool)usesSwipePad
{
    _usesSwipePad = usesSwipePad;
    _dpadView.image = [UIImage imageNamed:usesSwipePad? @"swipepad" : @"dpad"];
}

@end
