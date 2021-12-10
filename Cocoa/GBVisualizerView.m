#import "GBVisualizerView.h"
#import "GBPaletteEditorController.h"
#include <Core/gb.h>

#define SAMPLE_COUNT 1024

static NSColor *color_to_effect_color(typeof(GB_PALETTE_DMG.colors[0]) color)
{
    if (@available(macOS 10.10, *)) {
        double tint = MAX(color.r, MAX(color.g, color.b)) + 64;
        
        return [NSColor colorWithRed:color.r / tint
                               green:color.g / tint
                                blue:color.b / tint
                               alpha:tint/(255 + 64)];

    }
    return [NSColor colorWithRed:color.r / 255.0
                           green:color.g / 255.0
                            blue:color.b / 255.0
                           alpha:1.0];
}

@implementation GBVisualizerView
{
    GB_sample_t _samples[SAMPLE_COUNT];
    size_t _position;
}

- (void)drawRect:(NSRect)dirtyRect
{
    const GB_palette_t *palette = [GBPaletteEditorController userPalette];
    NSSize size = self.bounds.size;
    
    [color_to_effect_color(palette->colors[0]) setFill];
    NSRectFill(self.bounds);
    
    NSBezierPath *line = [NSBezierPath bezierPath];
    [line moveToPoint:NSMakePoint(0, size.height / 2)];
    
    for (unsigned i = 0; i < SAMPLE_COUNT; i++) {
        GB_sample_t *sample = _samples + ((i + _position) % SAMPLE_COUNT);
        double volume = ((signed)sample->left + (signed)sample->right) / 32768.0;
        [line lineToPoint:NSMakePoint(size.width * (i + 0.5) / SAMPLE_COUNT,
                                      (volume + 1) * size.height / 2)];
    }
    
    [line lineToPoint:NSMakePoint(size.width, size.height / 2)];
    [line setLineWidth:1.0];
    
    [color_to_effect_color(palette->colors[2]) setFill];
    [line fill];
    
    [color_to_effect_color(palette->colors[1]) setFill];
    NSRectFill(NSMakeRect(0, size.height / 2 - 0.5, size.width, 1));
    
    [color_to_effect_color(palette->colors[3]) setStroke];
    [line stroke];

    [super drawRect:dirtyRect];
}

- (void)addSample:(GB_sample_t *)sample
{
    _samples[_position++] = *sample;
    if (_position == SAMPLE_COUNT) {
        _position = 0;
    }
}

@end
