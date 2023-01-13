#import "GBViewBase.h"

@implementation GBViewBase
{
    uint32_t *_imageBuffers[3];
    unsigned _currentBuffer;
    GB_frame_blending_mode_t _frameBlendingMode;
}

- (void)screenSizeChanged
{
    if (_imageBuffers[0]) free(_imageBuffers[0]);
    if (_imageBuffers[1]) free(_imageBuffers[1]);
    if (_imageBuffers[2]) free(_imageBuffers[2]);
    
    size_t buffer_size = sizeof(_imageBuffers[0][0]) * GB_get_screen_width(_gb) * GB_get_screen_height(_gb);
    
    _imageBuffers[0] = calloc(1, buffer_size);
    _imageBuffers[1] = calloc(1, buffer_size);
    _imageBuffers[2] = calloc(1, buffer_size);
}

- (void)flip
{
    _currentBuffer = (_currentBuffer + 1) % self.numberOfBuffers;
}

- (unsigned) numberOfBuffers
{
    return _frameBlendingMode? 3 : 2;
}

- (void) createInternalView
{
    assert(false && "createInternalView must not be inherited");
}

- (uint32_t *)currentBuffer
{
    return _imageBuffers[_currentBuffer];
}

- (uint32_t *)previousBuffer
{
    return _imageBuffers[(_currentBuffer + 2) % self.numberOfBuffers];
}

- (uint32_t *) pixels
{
    return _imageBuffers[(_currentBuffer + 1) % self.numberOfBuffers];
}

- (void) setFrameBlendingMode:(GB_frame_blending_mode_t)frameBlendingMode
{
    _frameBlendingMode = frameBlendingMode;
    [self setNeedsDisplay];
}

- (GB_frame_blending_mode_t)frameBlendingMode
{
    if (_frameBlendingMode == GB_FRAME_BLENDING_MODE_ACCURATE) {
        if (!_gb || GB_is_sgb(_gb)) {
            return GB_FRAME_BLENDING_MODE_SIMPLE;
        }
        return GB_is_odd_frame(_gb)? GB_FRAME_BLENDING_MODE_ACCURATE_ODD : GB_FRAME_BLENDING_MODE_ACCURATE_EVEN;
    }
    return _frameBlendingMode;
}

- (void)dealloc
{
    free(_imageBuffers[0]);
    free(_imageBuffers[1]);
    free(_imageBuffers[2]);
}

#if !TARGET_OS_IPHONE
- (void)setNeedsDisplay
{
    [self setNeedsDisplay:true];
}
#endif
@end
