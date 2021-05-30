#import "GBOSDView.h"

@implementation GBOSDView
{
    bool _usesSGBScale;
    NSString *_text;
    double _animation;
    NSTimer *_timer;
}

- (void)setUsesSGBScale:(bool)usesSGBScale
{
    _usesSGBScale = usesSGBScale;
    [self setNeedsDisplay:true];
}

- (bool)usesSGBScale
{
    return _usesSGBScale;
}

- (void)displayText:(NSString *)text
{
    if (![[NSUserDefaults standardUserDefaults] boolForKey:@"GBOSDEnabled"]) return;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (![_text isEqualToString:text]) {
            [self setNeedsDisplay:true];
        }
        _text = text;
        self.alphaValue = 1.0;
        _animation = 2.5;
        // Longer strings should appear longer
        if ([_text rangeOfString:@"\n"].location != NSNotFound) {
            _animation += 4;
        }
        [_timer invalidate];
        self.hidden = false;
        _timer = [NSTimer scheduledTimerWithTimeInterval:0.025 target:self selector:@selector(animate) userInfo:nil repeats:true];
    });
}

- (void)animate
{
    _animation -= 0.1;
    if (_animation < 1.0) {
        self.alphaValue = _animation;
    };
    if (_animation == 0) {
        self.hidden = true;
        [_timer invalidate];
        _text = nil;
    }
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    if (!_text.length) return;
    
    double fontSize = 8;
    NSSize size = self.frame.size;
    if (_usesSGBScale) {
        fontSize *= MIN(size.width / 256, size.height / 224);
    }
    else {
        fontSize *= MIN(size.width / 160, size.height / 144);
    }
    
    NSFont *font = [NSFont boldSystemFontOfSize:fontSize];
    
    /* The built in stroke attribute uses an inside stroke, which is typographically terrible.
       We'll use a naïve manual stroke instead which looks better. */

    NSDictionary *attributes = @{
        NSFontAttributeName: font,
        NSForegroundColorAttributeName: [NSColor blackColor],
    };
        
    NSAttributedString *text = [[NSAttributedString alloc] initWithString:_text attributes:attributes];

    [text drawAtPoint:NSMakePoint(fontSize + 1, fontSize + 0)];
    [text drawAtPoint:NSMakePoint(fontSize - 1, fontSize + 0)];
    [text drawAtPoint:NSMakePoint(fontSize + 0, fontSize + 1)];
    [text drawAtPoint:NSMakePoint(fontSize + 0, fontSize - 1)];
    
    // The uses of sqrt(2)/2, which is more correct, results in severe ugly-looking rounding errors
    if (self.window.screen.backingScaleFactor > 1) {
        [text drawAtPoint:NSMakePoint(fontSize + 0.5, fontSize + 0.5)];
        [text drawAtPoint:NSMakePoint(fontSize - 0.5, fontSize + 0.5)];
        [text drawAtPoint:NSMakePoint(fontSize - 0.5, fontSize - 0.5)];
        [text drawAtPoint:NSMakePoint(fontSize + 0.5, fontSize - 0.5)];
    }
    
    attributes = @{
        NSFontAttributeName: font,
        NSForegroundColorAttributeName: [NSColor whiteColor],
    };

    text = [[NSAttributedString alloc] initWithString:_text attributes:attributes];
    
    [text drawAtPoint:NSMakePoint(fontSize, fontSize)];
}

@end
