#import <OpenGL/gl.h>
#import "GBView.h"
#import "GBButtons.h"
#import "NSString+StringForKey.h"

static GBShader *shader = nil;

@implementation GBView
{
    uint32_t *image_buffers[3];
    unsigned char current_buffer;
}

- (void) _init
{
    image_buffers[0] = malloc(160 * 144 * 4);
    image_buffers[1] = malloc(160 * 144 * 4);
    image_buffers[2] = malloc(160 * 144 * 4);
    _shouldBlendFrameWithPrevious = 1;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(filterChanged) name:@"GBFilterChanged" object:nil];
}

- (void) filterChanged
{
    [self setNeedsDisplay:YES];
    self.shader = nil;
}

- (void) setShouldBlendFrameWithPrevious:(BOOL)shouldBlendFrameWithPrevious
{
    _shouldBlendFrameWithPrevious = shouldBlendFrameWithPrevious;
    [self setNeedsDisplay:YES];
}

- (unsigned char) numberOfBuffers
{
    return _shouldBlendFrameWithPrevious? 3 : 2;
}

- (void)dealloc
{
    free(image_buffers[0]);
    free(image_buffers[1]);
    free(image_buffers[2]);
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (instancetype)initWithCoder:(NSCoder *)coder
{
    if (!(self = [super initWithCoder:coder]))
    {
        return self;
    }
    [self _init];
    return self;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if (!(self = [super initWithFrame:frameRect]))
    {
        return self;
    }
    [self _init];
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    if (!self.shader) {
        self.shader = [[GBShader alloc] initWithName:[[NSUserDefaults standardUserDefaults] objectForKey:@"GBFilter"]];
    }
    double scale = self.window.backingScaleFactor;
    if (_shouldBlendFrameWithPrevious) {
        [self.shader renderBitmap:image_buffers[current_buffer]
                         previous:image_buffers[(current_buffer + 2) % self.numberOfBuffers]
                           inSize:self.bounds.size
                            scale:scale];
    }
    else {
        [self.shader renderBitmap:image_buffers[current_buffer]
                         previous:NULL
                           inSize:self.bounds.size
                            scale:scale];
    }
    glFlush();
}


- (void) flip
{
    current_buffer = (current_buffer + 1) % self.numberOfBuffers;
    [self setNeedsDisplay:YES];
}

- (uint32_t *) pixels
{
    return image_buffers[(current_buffer + 1) % self.numberOfBuffers];
}

-(void)keyDown:(NSEvent *)theEvent
{
    bool handled = false;

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    for (GBButton i = 0; i < GBButtonCount; i++) {
        if ([[defaults stringForKey:button_to_preference_name(i)] isEqualToString:theEvent.charactersIgnoringModifiers]) {
            handled = true;
            switch (i) {
                case GBTurbo:
                    _gb->turbo = true;
                    break;
                    
                default:
                    _gb->keys[i] = true;
                    break;
            }
        }
    }

    if (!handled) {
        [super keyDown:theEvent];
    }
}

-(void)keyUp:(NSEvent *)theEvent
{
    bool handled = false;

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    for (GBButton i = 0; i < GBButtonCount; i++) {
        if ([[defaults stringForKey:button_to_preference_name(i)] isEqualToString:theEvent.charactersIgnoringModifiers]) {
            handled = true;
            switch (i) {
                case GBTurbo:
                    _gb->turbo = false;
                    break;

                default:
                    _gb->keys[i] = false;
                    break;
            }
        }
    }

    if (!handled) {
        [super keyUp:theEvent];
    }
}

-(void)reshape
{
    double scale = self.window.backingScaleFactor;
    glViewport(0, 0, self.bounds.size.width * scale, self.bounds.size.height * scale);
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}
@end
