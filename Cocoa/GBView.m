#import <Carbon/Carbon.h>
#import <OpenGL/gl.h>
#import "GBView.h"

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
    double scale = self.window.backingScaleFactor;
    glRasterPos2d(-1, 1);
    glPixelZoom(self.bounds.size.width / 160 * scale, self.bounds.size.height / -144 * scale);
    glDrawPixels(160, 144, GL_ABGR_EXT, GL_UNSIGNED_BYTE, image_buffers[current_buffer]);
    if (_shouldBlendFrameWithPrevious) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
        glBlendColor(1, 1, 1, 0.5);
        glDrawPixels(160, 144, GL_ABGR_EXT, GL_UNSIGNED_BYTE, image_buffers[(current_buffer + 2) % self.numberOfBuffers]);
        glDisable(GL_BLEND);
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
    unsigned short key = theEvent.keyCode;
    switch (key) {
        case kVK_RightArrow:
            _gb->keys[0] = true;
            break;
        case kVK_LeftArrow:
            _gb->keys[1] = true;
            break;
        case kVK_UpArrow:
            _gb->keys[2] = true;
            break;
        case kVK_DownArrow:
            _gb->keys[3] = true;
            break;
        case kVK_ANSI_X:
            _gb->keys[4] = true;
            break;
        case kVK_ANSI_Z:
            _gb->keys[5] = true;
            break;
        case kVK_Delete:
            _gb->keys[6] = true;
            break;
        case kVK_Return:
            _gb->keys[7] = true;
            break;
        case kVK_Space:
            _gb->turbo = true;
            break;

        default:
            [super keyDown:theEvent];
            break;
    }
}

-(void)keyUp:(NSEvent *)theEvent
{
    unsigned short key = theEvent.keyCode;
    switch (key) {
        case kVK_RightArrow:
            _gb->keys[0] = false;
            break;
        case kVK_LeftArrow:
            _gb->keys[1] = false;
            break;
        case kVK_UpArrow:
            _gb->keys[2] = false;
            break;
        case kVK_DownArrow:
            _gb->keys[3] = false;
            break;
        case kVK_ANSI_X:
            _gb->keys[4] = false;
            break;
        case kVK_ANSI_Z:
            _gb->keys[5] = false;
            break;
        case kVK_Delete:
            _gb->keys[6] = false;
            break;
        case kVK_Return:
            _gb->keys[7] = false;
            break;
        case kVK_Space:
            _gb->turbo = false;
            break;

        default:
            [super keyUp:theEvent];
            break;
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
