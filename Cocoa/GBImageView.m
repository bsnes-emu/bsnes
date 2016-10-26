#import "GBImageView.h"

@implementation GBImageViewGridConfiguration
- (instancetype)initWithColor:(NSColor *)color size:(NSUInteger)size
{
    self = [super init];
    self.color = color;
    self.size = size;
    return self;
}
@end

@implementation GBImageView
{
    NSTrackingArea *trackingArea;
}
- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetInterpolationQuality(context, kCGInterpolationNone);
    [super drawRect:dirtyRect];
    CGFloat y_ratio = self.frame.size.height / self.image.size.height;
    CGFloat x_ratio = self.frame.size.width / self.image.size.width;
    for (GBImageViewGridConfiguration *conf in self.verticalGrids) {
        [conf.color set];
        for (CGFloat y = conf.size * y_ratio; y < self.frame.size.height; y += conf.size * y_ratio) {
            NSBezierPath *line = [NSBezierPath bezierPath];
            [line moveToPoint:NSMakePoint(0, y + 0.5)];
            [line lineToPoint:NSMakePoint(self.frame.size.width, y + 0.5)];
            [line setLineWidth:1.0];
            [line stroke];
        }
    }
    
    for (GBImageViewGridConfiguration *conf in self.horizontalGrids) {
        [conf.color set];
        for (CGFloat x = conf.size * x_ratio; x < self.frame.size.width; x += conf.size * x_ratio) {
            NSBezierPath *line = [NSBezierPath bezierPath];
            [line moveToPoint:NSMakePoint(x + 0.5, 0)];
            [line lineToPoint:NSMakePoint(x + 0.5, self.frame.size.height)];
            [line setLineWidth:1.0];
            [line stroke];
        }
    }
}

- (void)setHorizontalGrids:(NSArray *)horizontalGrids
{
    self->_horizontalGrids = horizontalGrids;
    [self setNeedsDisplay];
}

- (void)setVerticalGrids:(NSArray *)verticalGrids
{
    self->_verticalGrids = verticalGrids;
    [self setNeedsDisplay];
}

- (void)updateTrackingAreas
{
    if(trackingArea != nil) {
        [self removeTrackingArea:trackingArea];
    }
    
    trackingArea = [ [NSTrackingArea alloc] initWithRect:[self bounds]
                                                 options:NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways | NSTrackingMouseMoved
                                                   owner:self
                                                userInfo:nil];
    [self addTrackingArea:trackingArea];
}

- (void)mouseExited:(NSEvent *)theEvent
{
    if ([self.delegate respondsToSelector:@selector(mouseDidLeaveImageView:)]) {
        [self.delegate mouseDidLeaveImageView:self];
    }
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    if ([self.delegate respondsToSelector:@selector(imageView:mouseMovedToX:Y:)]) {
        NSPoint location = [self convertPoint:theEvent.locationInWindow fromView:nil];
        location.x /= self.bounds.size.width;
        location.y /= self.bounds.size.height;
        location.y = 1 - location.y;
        location.x *= self.image.size.width;
        location.y *= self.image.size.height;
        [self.delegate imageView:self mouseMovedToX:(NSUInteger)location.x Y:(NSUInteger)location.y];
    }
}

@end
