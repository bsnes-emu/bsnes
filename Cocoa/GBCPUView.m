#import "GBCPUView.h"

#define SAMPLE_COUNT 0x100 // ~4 seconds

@implementation GBCPUView
{
    double _samples[SAMPLE_COUNT];
    size_t _position;
}

- (void)drawRect:(NSRect)dirtyRect
{
    NSSize size = self.bounds.size;
    
    NSBezierPath *line = [NSBezierPath bezierPath];
    {
        double sample = _samples[_position % SAMPLE_COUNT];
        [line moveToPoint:NSMakePoint(size.width * 0.5 / SAMPLE_COUNT,
                                      sample * size.height)];
    }
    
    for (unsigned i = 1; i < SAMPLE_COUNT; i++) {
        double sample = _samples[(i + _position) % SAMPLE_COUNT];
        [line lineToPoint:NSMakePoint(size.width * (i + 0.5) / SAMPLE_COUNT,
                                      sample * size.height)];
    }
    
    NSBezierPath *fill = [line copy];
    [fill lineToPoint:NSMakePoint(size.width + 0.5, 0)];
    [fill lineToPoint:NSMakePoint(0.5, 0)];
    
    NSColor *strokeColor;
    if (@available(macOS 10.10, *)) {
        strokeColor = [NSColor systemGreenColor];
    }
    else {
        strokeColor = [NSColor colorWithRed:3.0 / 16 green:0.5 blue:5.0 / 16 alpha:1.0];
    }
    NSColor *fillColor = [strokeColor colorWithAlphaComponent:1 / 3.0];
    
    [fillColor setFill];
    [fill fill];
    
    
    [strokeColor setStroke];
    [line stroke];

    [super drawRect:dirtyRect];
}

- (void)addSample:(double)sample
{
    _samples[_position++] = sample;
    if (_position == SAMPLE_COUNT) {
        _position = 0;
    }
}

@end
